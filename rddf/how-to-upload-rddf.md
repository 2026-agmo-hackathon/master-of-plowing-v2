# Please run the following commands.

```bash
# Grant execute permission (once only)
chmod a+x ./upload_rddf.sh

# Upload
./upload_rddf.sh \
  --env ./participant-env.json \
  -f ./field_m1.rddf \
  --feu-id ${FEU_ID} \
  --feature-id ${FEATURE_ID}
```

`--env` is required. It points at the Postman environment JSON issued by the
organizers, which supplies `tokenUrl`, `baseUrl`, `cp_client_id`,
`cp_client_secret`, `feature_id` and `feu_id`. `--feature-id` and `--feu-id`
are optional overrides for the values in that file. `jq` and `curl` must be
installed.

# FEU_ID example
abcdefgh-abcd-abcd-1234-abcd1234abcd

# FEATURE_ID example
10234dev
