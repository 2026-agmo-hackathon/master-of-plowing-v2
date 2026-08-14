#!/usr/bin/env bash
set -euo pipefail

# Upload an RDDF through the participant Control Plane account. Credentials are
# deliberately read from a caller-supplied environment export; participant
# source archives must never embed reusable client secrets.

ENV_FILE=""
FILE_PATH=""
FEATURE_ID_OVERRIDE=""
FEU_ID_OVERRIDE=""

usage() {
  cat >&2 <<'EOF'
Usage: upload_rddf.sh --env <postman-environment.json> --file <route.rddf>
                      [--feature-id <id>] [--feu-id <id>]
EOF
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    -e|--env)        ENV_FILE="$2"; shift 2 ;;
    -f|--file)       FILE_PATH="$2"; shift 2 ;;
    --feature-id)    FEATURE_ID_OVERRIDE="$2"; shift 2 ;;
    --feu-id)        FEU_ID_OVERRIDE="$2"; shift 2 ;;
    -h|--help)       usage; exit 0 ;;
    *) echo "Unknown argument: $1" >&2; usage; exit 2 ;;
  esac
done

[[ -n "$ENV_FILE" ]] || { echo "Error: --env is required" >&2; exit 2; }
[[ -f "$ENV_FILE" ]] || { echo "Error: env file not found: $ENV_FILE" >&2; exit 1; }
[[ -n "$FILE_PATH" ]] || { echo "Error: --file is required" >&2; exit 2; }
[[ -f "$FILE_PATH" ]] || { echo "Error: file not found: $FILE_PATH" >&2; exit 1; }
command -v jq >/dev/null || { echo "Error: jq is required" >&2; exit 1; }
command -v curl >/dev/null || { echo "Error: curl is required" >&2; exit 1; }

get_var() {
  jq -r --arg key "$1" \
    '.values[] | select(.enabled and .key == $key) | .value' "$ENV_FILE"
}

TOKEN_URL="$(get_var tokenUrl)"
BASE_URL="$(get_var baseUrl)"
CP_CLIENT_ID="$(get_var cp_client_id)"
CP_CLIENT_SECRET="$(get_var cp_client_secret)"
FEATURE_ID="${FEATURE_ID_OVERRIDE:-$(get_var feature_id)}"
FEU_ID="${FEU_ID_OVERRIDE:-$(get_var feu_id)}"

for name in TOKEN_URL BASE_URL CP_CLIENT_ID CP_CLIENT_SECRET FEATURE_ID FEU_ID; do
  [[ -n "${!name}" ]] || { echo "Error: '$name' is empty in $ENV_FILE" >&2; exit 1; }
done

token_response="$(curl -fsS -X POST "$TOKEN_URL" \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  --data-urlencode 'grant_type=client_credentials' \
  --data-urlencode "client_id=$CP_CLIENT_ID" \
  --data-urlencode "client_secret=$CP_CLIENT_SECRET")"
token="$(jq -r '.access_token // empty' <<<"$token_response")"
[[ -n "$token" ]] || { echo "Error: token response had no access_token" >&2; exit 1; }

upload_url="$BASE_URL/api/v1/features/$FEATURE_ID/feu/$FEU_ID/files"
response_file="$(mktemp)"
trap 'rm -f "$response_file"' EXIT
status="$(curl -sS -o "$response_file" -w '%{http_code}' \
  -X POST "$upload_url" \
  -H "Authorization: Bearer $token" \
  -F "file=@$FILE_PATH")"

if [[ "$status" =~ ^2 ]]; then
  echo "Uploaded $FILE_PATH (HTTP $status)."
else
  echo "Upload failed (HTTP $status): $(<"$response_file")" >&2
  exit 1
fi
