# SeamOS Hackathon 2026: The Master of Plowing — Teilnehmer-Leitfaden

> Dieses Dokument ist ein praxisorientierter Leitfaden für Teilnehmer des SeamOS Hackathon 2026.
> Hintergrundinformationen zum Wettbewerb und Bewertungskriterien findest du im separaten Konzeptdokument. Dieses Dokument behandelt den vollständigen Ablauf: **Repository-Setup → RDDF erstellen → im App anwenden**.

---

## 1. Wettbewerbsübersicht

- **Mission**: Steuere einen virtuellen Traktor und pflüge die Fläche **„Alpha"** in **kürzester Zeit + maximaler Flächenabdeckung**
- **Harte Einschränkung**: Maximale Fahrgeschwindigkeit **7 km/h** (Werte darüber werden abgelehnt, nicht still begrenzt)
- **Strafen (insgesamt 3 Arten)**
  - **Rückwärtsfahrt (Reverse)** auf dem Feld — der betroffene Bereich wird als ungepflügter Bereich zurückgesetzt
  - **Verlassen des Felds** — der Timer läuft **10-fach schneller** (solange der Traktor außerhalb der Grenze ist)
  - **Mittlerer Fehler bei Abschluss** — der kumulative Pfadfehler (RMS) wird zur Endzeit addiert
- **Bewertung**: **Gesamtzeit** und **Flächenanteil (ungepflügte Bereiche minimieren)** fließen beide in die Punktzahl ein.
  - Es gibt **keinen harten Cutoff** (z. B. 95%) — auch bei niedrigem Flächenanteil ist eine Ergebnisabgabe möglich
  - Ein niedrigerer Flächenanteil wirkt sich jedoch nachteilig auf die Punktzahl aus

> **Kernaussage**: Was Teilnehmer erstellen und einreichen, ist eine **RDDF (Route Definition Data File)**.
> Der Traktor-Steuerungsalgorithmus selbst wird vom bereitgestellten Skeleton übernommen — Teilnehmer formulieren die **optimale Route zum Pflügen des Felds** als RDDF und reichen diese ein.

---

## 2. Voraussetzungen

| Komponente | Hinweis |
|------|------|
| **SeamOS IDE (FeatureDesigner)** | Distributionsversion der Organisatoren verwenden |
| **Claude Code** | Aktuelle Version von der offiziellen Website installieren |
| **SeamOS Everywhere** | Distributionsversion der Organisatoren verwenden |


---

## 3. Repository-Struktur

```
master_of_plow/
├── com.bosch.fsp.master_of_plow/   # FSP (Feature Spec Project)
├── master_of_plow_app/             # App-Hauptteil (C++-Code, Konfiguration)
│   └── src-gen/AppMain/tracking/   # Pfadverfolgungs-Skelett — Referenz zum Lesen (§10)
├── master_of_plow_CPP_SDK/         # SeamOS C++ SDK (bereitgestellt)
├── customui-src/                   # Dashboard-Quelle (React) — baut nach master_of_plow_app/ui/
├── rddf/                           # RDDF-Bereich (Erstellen & Validieren durch Teilnehmer)
├── docs/                           # Ablauf der Fahrt-Transaktion und FIF-Validierung
└── HACKATHON_GUIDE.md              # Dieses Dokument
```

### 3.1 Öffnen in der SeamOS IDE

1. SeamOS IDE starten
2. **File → Open Project...**
3. Pfad zum geklonten `master_of_plow/`-Verzeichnis angeben und öffnen

### 3.2 Build-Verifikation

Verwende die SeamOS IDE, um das Projekt zu bauen und durch App-Start sowie Testausführung zu verifizieren, dass die Umgebung korrekt eingerichtet ist.

#### 1. Build durchführen

Klicke in der oberen Toolbar auf **Build** und dann auf **Build all**, um den vollständigen Projekt-Build auszuführen.
- SDK (SeamOS C++ SDK)
- App (App-Hauptteil)
- Test (lokales Testmodul)

Alle drei Module müssen erfolgreich gebaut werden.

#### 2. Paketieren, installieren und prüfen

Führe die lokalen Tests aus und erzeuge dann mit dem vom Veranstalter
bereitgestellten SDK/Feature Designer (oder dessen CI-Pipeline) das FIF.
Installiere das FIF über den normalen Emulator-Workflow; der Benutzer wählt und
startet anschließend das Feature. Das direkte Kopieren einer ausführbaren Datei
ist kein unterstützter Build-/Validierungsweg. Siehe
`docs/official-fif-validation-workflow.md`.

---

## 4. Was ist RDDF?

**RDDF (Road Definition Data File)** ist eine **Waypoint-Sequenz**, der der Traktor folgt.
Es ist das wesentliche Ergebnis, das Teilnehmer im Wettbewerb einreichen, und definiert:

- Wohin gefahren wird (Breiten-/Längengrad)
- Wie schnell gefahren wird (Geschwindigkeit)
- Vorwärts- oder Rückwärtsfahrt (Vorzeichen der Geschwindigkeit)
- Ob der Pflug an diesem Punkt **abgesenkt oder angehoben** wird (implement flag)

### 4.1 Dateiformat

**9 Spalten**, getrennt durch Tabulator (`\t`), ein Waypoint pro Zeile:

```
lineNo  index  lat         lon          res1  res2  res3  speed  implementFlag
1       1      35.8000317  126.8807033  0.0   0.0   0.0   3.00   1
2       2      35.8000317  126.8807141  0.0   0.0   0.0   3.00   1
...
22      22     35.8001131  126.8808124  0.0   0.0   0.0   -1.00  0
23      23     35.8001040  126.8808124  0.0   0.0   0.0   -1.00  0
```

| Spalte | Bedeutung |
|------|------|
| `lineNo` | Zeilennummer in der Datei (aufsteigend ab 1) |
| `index` | Waypoint-Index (normalerweise identisch mit lineNo) |
| `lat` | Breitengrad (Dezimalgrad, WGS84) |
| `lon` | Längengrad (Dezimalgrad, WGS84) |
| `res1`, `res2`, `res3` | Reservierte Felder — immer `0.0` |
| `speed` | Zielgeschwindigkeit (km/h). **Negativ = Rückwärtsfahrt** |
| `implementFlag` | `1` = Pflug abgesenkt (plowing on), `0` = Pflug angehoben (turn/idle) |

### 4.2 Geschwindigkeitsregeln

- `0 < speed ≤ 7.0` — Vorwärtsfahrt; unter `2,05 km/h` wird mit Warnung geladen
- `speed = 0` — expliziter Stopp-Waypoint
- `-7.0 ≤ speed < 0` — Rückwärtsfahrt (Wettbewerbs-Penalty); Beträge unter `2,05 km/h` erzeugen eine Warnung
- Wenn das Vorzeichen der Geschwindigkeit zwischen benachbarten Waypoints wechselt, liegt ein **Gangwechselpunkt** vor — die Waypoints müssen so angeordnet sein, dass die Fahrtrichtung (Heading) umgekehrt wird

### 4.3 implementFlag

- Beim Pflügen auf einer Spur: `1`
- Beim Wenden im Vorgewende (headland) am Feldrand: `0`
- **Rückwärtsabschnitte immer `0`** — beim Rückwärtsfahren wird nicht gepflügt.

### 4.4 Automatische Validierung

RDDF-Dateien werden sofort nach Empfang aus der Cloud automatisch validiert. Formatfehler und Verstöße gegen Sicherheitsobergrenzen werden **nicht gespeichert und abgelehnt**; physikalische Nachführgrenzen werden unverändert geladen und als Warnung gemeldet. Die Validierungslogik ist in `RddfValidator.cpp` definiert.

| Regel | Inhalt | Bei Verstoß |
|------|------|---------|
| **(1) Leere Datei** | 0 Waypoints | abgelehnt |
| **(2) Geschwindigkeitsbereich** | `\|speed\| ≤ 7,0 km/h`; ein Nicht-null-Betrag unter `2,05 km/h` kann nicht dauerhaft gehalten werden | Oberhalb der Grenze ablehnen; niedrige Geschwindigkeit unverändert laden und warnen |
| **(3) Waypoint-Abstand** | Benachbarte Punkte erfüllen den dokumentierten Mindest-/Höchstabstand | Ablehnung mit dem betroffenen Paar |
| **(4) Physikalische Krümmung** | Lokale Geometrie mit der gemessenen Lenkgrenze vergleichen | Unverändert laden und vor der Nachführgrenze warnen |

#### Verhalten bei Ablehnung

- Im Simulator-UI wird sofort ein **„RDDF Validation Error"-Dialog** mit dem genauen Ablehnungsgrund angezeigt.
- Abgelehnte RDDFs gelangen nicht in die Pending-Liste und werden automatisch gelöscht — **korrigiere die Datei und lade sie erneut hoch**.
- Wenn kein Dialog erscheint und die RDDF normal in der Pending-Liste landet, gilt sie als validiert.

#### Startpose als Sicherheitsprüfung der Sitzung

Die statische Dateivalidierung erfindet keinen StartPoint. Der Loader besitzt
keine autoritative Live-Map-Spawnposition. Vor der Bewegungsfreigabe muss die
Simulator-/Sitzungsprüfung deshalb die frische Fahrzeugpose mit Waypoint 0
vergleichen.

> Hinweis: Der Loader bewahrt den eingereichten Geschwindigkeitswert exakt.
> Nur Beträge über `7,0 km/h` werden abgelehnt. Nicht-null-Werte unter
> `2,05 km/h` werden unverändert geladen; eine angewandte Kriechgrenze erscheint
> in der Laufzeit-Telemetrie.

### 4.5 Karteninformationen (Field Maps)

Im Wettbewerb werden **3 öffentliche Karten (M1, M2, M3)** verwendet. Das Polygon (Feldgrenze) jeder Karte ist in WGS84-Koordinaten (`lat, lon`) definiert — die RDDF der Teilnehmer soll das Innere des jeweiligen Polygons möglichst vollständig abdecken.

| Karte | Name | Origin (lat, lon) | Fläche | Eckpunkte |
|----|------|-------------------|------|-----------|
| **M1** | Open Fields | 35.8000, 126.8800 | 1.500 m² | 4 |
| **M2** | Sloped Acres | 34.6800, 126.9100 | 1.948 m² | 37 |
| **M3** | Patchwork Plots | 35.4200, 127.3900 | 1.355 m² | 54 |

> Origin ist der GPS-Referenzpunkt für den lokalen Ursprung (0, 0) des Polygons (zur Referenz); die Fläche ist der per Shoelace-Formel berechnete m²-Wert.

#### M1 · Open Fields (1.500 m², 4 Eckpunkte)

![M1 Open Fields](docs/maps/map1_polygon.png)

Einfachstes rechteckiges Feld. Ideal, um den Boustrophedon-Algorithmus schnell zu testen.

```
polygon = [
  (35.8002897, 126.8806012), (35.8004622, 126.8802035),
  (35.8001628, 126.8800000), (35.8000000, 126.8803803),
]
```

#### M2 · Sloped Acres (1.948 m², 37 Eckpunkte)

![M2 Sloped Acres](docs/maps/map2_polygon.png)

Komplexes Polygon, das ein Hangfeld simuliert. Der Vorgewende-Bereich (headland) ist lang, was U-turn-Behandlung anspruchsvoll macht.

```
polygon = [
  (34.6805868, 126.9102540), (34.6804636, 126.9100401),
  (34.6804514, 126.9100264), (34.6804341, 126.9100117),
  (34.6804101, 126.9100000), (34.6803773, 126.9100074),
  (34.6803447, 126.9100296), (34.6803099, 126.9100506),
  (34.6802487, 126.9100895), (34.6801798, 126.9101240),
  (34.6800194, 126.9101844), (34.6800025, 126.9101929),
  (34.6800000, 126.9102047), (34.6800011, 126.9102220),
  (34.6800076, 126.9102429), (34.6800240, 126.9102651),
  (34.6800455, 126.9102861), (34.6800750, 126.9103133),
  (34.6801236, 126.9103631), (34.6801328, 126.9103724),
  (34.6801435, 126.9103860), (34.6801583, 126.9104057),
  (34.6801838, 126.9104396), (34.6802037, 126.9104747),
  (34.6802139, 126.9104994), (34.6802247, 126.9105198),
  (34.6802354, 126.9105480), (34.6802425, 126.9105771),
  (34.6802543, 126.9106086), (34.6802578, 126.9106314),
  (34.6802619, 126.9106436), (34.6802696, 126.9106499),
  (34.6802767, 126.9106461), (34.6802850, 126.9106363),
  (34.6803441, 126.9105654), (34.6803610, 126.9105499),
  (34.6803819, 126.9105290),
]
```

#### M3 · Patchwork Plots (1.355 m², 54 Eckpunkte)

![M3 Patchwork Plots](docs/maps/map3_polygon.png)

Unregelmäßiges Feld mit konkaven und konvexen Bereichen. Mit einem einfachen Boustrophedon-Ansatz verbleiben viele ungepflügte Bereiche — es sind Polygon-Clipping und bereichsspezifische Verarbeitung erforderlich.

```
polygon = [
  (35.4205363, 127.3902241), (35.4205148, 127.3901085),
  (35.4205020, 127.3900000), (35.4202816, 127.3900305),
  (35.4202305, 127.3900267), (35.4201433, 127.3900066),
  (35.4201374, 127.3900667), (35.4201845, 127.3900979),
  (35.4202004, 127.3901073), (35.4202145, 127.3901207),
  (35.4202350, 127.3901547), (35.4202510, 127.3901941),
  (35.4202633, 127.3902458), (35.4202652, 127.3902776),
  (35.4202611, 127.3903620), (35.4201999, 127.3903605),
  (35.4201579, 127.3903998), (35.4201428, 127.3904155),
  (35.4201145, 127.3904243), (35.4200712, 127.3904327),
  (35.4200707, 127.3904499), (35.4200667, 127.3904538),
  (35.4200640, 127.3904632), (35.4200598, 127.3904727),
  (35.4200525, 127.3904733), (35.4200484, 127.3904905),
  (35.4200105, 127.3904933), (35.4200014, 127.3905578),
  (35.4200000, 127.3905707), (35.4200178, 127.3905879),
  (35.4200319, 127.3906068), (35.4200430, 127.3906307),
  (35.4200766, 127.3906202), (35.4201037, 127.3906140),
  (35.4201269, 127.3906073), (35.4201511, 127.3905483),
  (35.4201598, 127.3905333), (35.4201770, 127.3905294),
  (35.4202127, 127.3905333), (35.4202578, 127.3905433),
  (35.4202830, 127.3904800), (35.4202971, 127.3904454),
  (35.4203168, 127.3904127), (35.4203341, 127.3903826),
  (35.4203501, 127.3903605), (35.4203743, 127.3903364),
  (35.4203994, 127.3903132), (35.4204189, 127.3902909),
  (35.4204427, 127.3902630), (35.4204637, 127.3902486),
  (35.4204810, 127.3902407), (35.4205012, 127.3902347),
  (35.4205213, 127.3902308), (35.4205394, 127.3902358),
]
```

---

## 5. RDDF-Erstellungs-Workflow

### 5.1 Verzeichnisstruktur

```
master_of_plow/rddf/
├── 1.rddf                  # Route für Feld 1
├── 2.rddf                  # Route für Feld 2
├── 3.rddf                  # Route für Feld 3
├── upload_rddf.sh          # Cloud-Upload-Skript
└── how-to-upload-rddf.md   # Hinweise zu Upload-Befehlen
```

### 5.2 RDDF manuell erstellen

In der einfachsten Form schreibst du tab-getrennte Daten direkt in einem Texteditor.

```
1	1	35.8000317	126.8807033	0.0	0.0	0.0	3.00	1
2	2	35.8000317	126.8807141	0.0	0.0	0.0	3.00	1
3	3	35.8000317	126.8807249	0.0	0.0	0.0	3.00	1
```

> **Achtung**: Als Trennzeichen muss zwingend der **Tabulator (`\t`)** verwendet werden. Leerzeichen im Trennfeld führen zu einer Ablehnung durch den Parser.

### 5.3 RDDF per Code generieren

Dutzende bis Hunderte von Waypoints von Hand einzutragen ist unpraktikabel. Üblicherweise schreibt man ein Skript, das **Feldgeometrie + Algorithmusparameter** (Spurabstand, Wendekreisradius usw.) als Eingabe nimmt und das RDDF generiert. Die Programmiersprache ist frei wählbar — dieser Leitfaden zeigt ein Python-Beispiel.

#### Beispiel: Boustrophedon (Hin- und Rückfahrt) + Halbkreis-U-turn

Das Feld wird in parallele Spuren im SWATH-Abstand (Pflugbreite) aufgeteilt, und gerade/ungerade Spuren werden abwechselnd abgefahren. An den Enden erfolgt der Übergang zur nächsten Spur über einen **vorwärts fahrenden Halbkreis-U-turn**. Da keine Rückwärtsfahrt verwendet wird, wird die **Penalty durch ungepflügte-Bereich-Reset vermieden**.

```
↑ Spur 0       U-turn ↓ Spur 1       U-turn ↑ Spur 2  ...
│             ╲          │            ╱          │
│              ╲         │           ╱           │
│           Halbkreis-U-turn   │       Halbkreis-U-turn      │
```

#### Minimales funktionsfähiges Beispielskript (`gen_rddf.py`)

```python
"""
gen_rddf.py — Beispiel zur RDDF-Generierung im Boustrophedon-Muster
Teilnehmer sollen dieses Skript als Ausgangspunkt verwenden und mit
eigenen Algorithmen weiterentwickeln.
"""

import math

# --- Feldursprung (unten links) und Umrechnung in Lat/Lon ---
LAT0, LON0 = 35.8001, 126.8807
M_PER_DEG_LAT = 110540.0
M_PER_DEG_LON = 111320.0 * math.cos(math.radians(LAT0))

def to_latlon(x_m, y_m):
    return (LAT0 + y_m / M_PER_DEG_LAT,
            LON0 + x_m / M_PER_DEG_LON)

# --- Parameter ---
FIELD_W = 30.0     # Feldbreite Ost-West (m)
FIELD_H = 50.0     # Feldlänge Nord-Süd (m)
SWATH   = 4.0      # Spurabstand = Pflugbreite (m)
STEP    = 1.0      # Waypoint-Abstand (m)
V_LANE  = 3.0      # Spurgeschwindigkeit (km/h)
V_TURN  = 1.5      # Wendegeschwindigkeit (km/h)
N_ARC   = 16       # Anzahl der Segmente für den Halbkreis-U-turn

waypoints = []  # (x, y, speed, implementFlag)
r = SWATH / 2.0
n_lanes = int((FIELD_W - SWATH) // SWATH) + 1

for k in range(n_lanes):
    x = SWATH / 2.0 + k * SWATH
    going_up = (k % 2 == 0)

    # (1) Gerade Spur — Pflug aktiv (ON)
    y0, y1 = (r, FIELD_H - r) if going_up else (FIELD_H - r, r)
    n_steps = max(2, int(abs(y1 - y0) / STEP) + 1)
    for i in range(n_steps):
        t = i / (n_steps - 1)
        waypoints.append((x, y0 + (y1 - y0) * t, V_LANE, 1))

    # (2) Halbkreis-U-turn zur naechsten Spur — Pflug inaktiv (OFF)
    if k == n_lanes - 1:
        break
    cx, cy = x + r, y1
    th0 = math.pi
    th1 = 0.0 if going_up else 2.0 * math.pi   # oberer ↗ / unterer ↘ Halbkreis
    for j in range(1, N_ARC + 1):
        theta = th0 + (th1 - th0) * j / N_ARC
        waypoints.append((cx + r * math.cos(theta),
                          cy + r * math.sin(theta),
                          V_TURN, 0))

# --- RDDF-Dateiausgabe (Tabulator-getrennt, 9 Spalten) ---
with open("alpha.rddf", "w") as f:
    for idx, (x, y, v, flag) in enumerate(waypoints, 1):
        lat, lon = to_latlon(x, y)
        f.write(f"{idx}\t{idx}\t{lat:.7f}\t{lon:.7f}\t0.0\t0.0\t0.0\t{v:.2f}\t{flag}\n")

print(f"{len(waypoints)} waypoints → alpha.rddf")
```

#### Punkte zur Weiterentwicklung mit eigenem Algorithmus

Das obige Skript behandelt nur **rechteckige Felder mit gleichmäßigen Spuren**. Im tatsächlichen Wettbewerb müssen folgende Punkte durch eigene Algorithmen ersetzt werden:

- **Polygon-Verarbeitung der Feldgrenze** — Spurlängen an die tatsächliche Feldform (kein Rechteck) anpassen
- **Separater Umgang mit dem Vorgewende (headland)** — wenn der Halbkreis-U-turn über den Feldrand hinausgeht, greift die **Timer-10-fach-Beschleunigung** als Penalty
- **Optimierung von Spurabstand / Wendekreisradius** — Mindestwendekreis des Traktors ≈ 3,28 m; SWATH sollte größer sein, um einen gleichmäßigen U-turn zu ermöglichen
- **Start- und Endpunkt** — Entwurf der Einfahrtroute vom Punkt A zur ersten Spur
- **Minimierung ungepflügter Bereiche** — kleine ungepflügte Bereiche an Ecken und Rändern separat behandeln

### 5.4 Validierungs-Checkliste

Prüfe folgende Punkte, bevor du das RDDF hochlädst.

- [ ] Alle Zeilen konsistent mit 9 Spalten und **Tabulator-Trennzeichen**?
- [ ] `lineNo` beginnt bei 1 und steigt lückenlos an?
- [ ] Startpose von Karte/Sitzung vor der Bewegungsfreigabe an Waypoint 0 ausgerichtet? (§4.4)
- [ ] Hat jeder `speed`-Wert einen Absolutwert von **höchstens 7,0 km/h** (unter `2,05 km/h` erfolgt eine Warnung)?
- [ ] Waypoint-Abstände angemessen? — **Bei geraden Abschnitten genügen 2 Punkte (Start + Ende)**, **bei Kurvenabschnitten ausreichend dicht**
- [ ] Keine Abschnitte mit `speed < 0` auf dem gepflügten Feld? (Rückwärtsfahrt-Penalty vermeiden)
- [ ] Bei Rückwärts → Vorwärts-Wechsel: **Heading umgekehrt**? (Drehung auf der Stelle nicht möglich)
- [ ] `implementFlag` aller Rückwärtsabschnitte ist `0`?
- [ ] U-turn-Pfad weicht nicht stark von der Feldgrenze ab? (10-fach-Beschleunigung Penalty)

---

## 6. RDDF in der App anwenden

### 6.1 Cloud-Upload

Die Wettbewerbsbewertung basiert auf dem **in der Cloud hochgeladenen RDDF**. Verwende die von den Organisatoren ausgestellten Werte für `FEU_ID` und `FEATURE_ID`.

```bash
cd master_of_plow/rddf

# Ausführungsberechtigung erteilen (einmalig)
chmod +x ./upload_rddf.sh

# Hochladen
./upload_rddf.sh \
  -f ./1.rddf \
  --feu-id <YOUR_FEU_ID> \
  --feature-id <YOUR_FEATURE_ID>
```

Beispiel einer erfolgreichen Ausgabe:

```
[1/2] Requesting CP token from https://... ...
  provider_id (sub) = ...
[2/2] Uploading './1.rddf'
       -> .../api/v1/features/<FEATURE_ID>/feu/<FEU_ID>/files
  Status: 200
Done.
```

#### Parameterformat

```
FEU_ID Beispiel      : abcdefgh-abcd-abcd-1234-abcd1234abcd
FEATURE_ID Beispiel  : 10234dev
```

#### Häufige Fehler

| Symptom | Ursache / Lösung |
|------|-------------|
| `jq is required` | `sudo apt install jq` oder `brew install jq` |
| `Status: 401` | Tippfehler in FEU_ID / FEATURE_ID oder abgelaufen — Organisatoren kontaktieren |
| `Status: 4xx` (Datei abgelehnt) | RDDF-Formatfehler — Checkliste §5.4 erneut prüfen |
| `Status: 5xx` | Vorübergehender Serverfehler — kurz warten und erneut versuchen, bei Anhaltung Organisatoren kontaktieren |

### 6.2 Prüfung nach dem Upload

Nach dem Upload empfängt die App das RDDF über den `CloudDownloadListener` und verarbeitet es mit dem `RddfParser`. Prüfe im Simulator, ob der Traktor der vorgesehenen Route folgt.

---

## 7. Wettbewerbsregeln

### 7.1 Missionsbedingungen

| Punkt | Wert |
|------|-----|
| Startpunkt | **Position des ersten Waypoints im RDDF** (beim Map Switch wird der Traktor auf diese Position ausgerichtet) |
| Zielfeld | **M1 · Open Fields**, **M2 · Sloped Acres**, **M3 · Patchwork Plots** — 3 öffentliche Karten (siehe §4.5) |
| Zielvorgabe | **Kein harter Cutoff** — Flächenanteil fließt proportional in die Punktzahl ein |
| Maximalgeschwindigkeit | **7 km/h** |
| Overlap | Erlaubt (kein Punktabzug) |

### 7.2 Strafen

Es gibt drei Strafen, die die Wettbewerbsbewertung beeinflussen.

#### (1) Rückwärtsfahrt auf dem Feld — ungepflügter Bereich zurückgesetzt

Das Pflügen an der Position der Rückwärtsfahrt wird ungültig — der Bereich wird wieder als ungepflügt markiert.
→ Es müssen effiziente Wendemuster auf Vorwärtsbasis (z. B. U-turn, Omega-Wende) entworfen werden.

#### (2) Verlassen des Felds — Timer 10-fach beschleunigt

Solange sich der Traktor **außerhalb des Felds (Alpha-Grenze)** befindet, läuft der Simulations-Timer **10-mal schneller**.
Aus Sicht der Teilnehmer bedeutet das: Jede Sekunde außerhalb des Felds entspricht 10 Sekunden auf der Zeitmessung.

→ Beim Wenden im Vorgewende (headland), wenn die Route über die Feldgrenze hinausgeht, entsteht ein großer Zeitverlust. Wendemuster, die vollständig innerhalb der Feldgrenze abgeschlossen werden, sind vorteilhaft.

#### (3) Mittlerer Fehler bei Abschluss — Penalty auf die Endzeit

Der während der Fahrt kumulierte **Pfadfolgefehler (Cross-Track Error, RMS)** wird proportional als Penalty-Sekunden zur Endzeit addiert.

Code-Referenz (`MainControllerImpl.cpp`, Abschnitt `track_complete`):

```cpp
RunSummary& rs = getRunSummaryMut();
rs.elapsedS   = elapsedSAtComplete_;
rs.penaltyS   = rs.deviationM * 1000.0;   // RMS CTE(m) × 1000
rs.finalTimeS = rs.elapsedS + rs.penaltyS;
```

- `deviationM` — kumulierter RMS-Wert des CTE (Root Mean Square) pro Tick, Einheit Meter
- `penaltyS` — `deviationM × 1000` Sekunden
- `finalTimeS` — `elapsedS + penaltyS`, **Leaderboard-Endergebnis**

→ Bei einem mittleren CTE von **0,1 m** kommen **100 Sekunden** Penalty hinzu, bei **0,5 m** sind es **500 Sekunden**.
→ Wenn Kurvenabschnitte zu dünn besetzt sind oder abrupte Heading-Wechsel auftreten, steigt der Pure Pursuit-Verfolgungsfehler stark an und die Penalty explodiert.

##### Tipps zur Reduzierung des mittleren Fehlers

- **Gerade Abschnitte** benötigen nur **2 Punkte** (Start + Ende) — der Tracker folgt der Linie zwischen zwei Punkten, Zwischenpunkte sind nicht nötig.
- **Kurvenabschnitte** sollten ausreichend dicht besetzt sein — je größer die Krümmung, desto feiner die Unterteilung.
- Heading-Änderungen zwischen benachbarten Waypoints klein halten (kein großer Winkelsprung auf einmal, z. B. unter 15°)
- Es wird empfohlen, im eigenen Generator eine Validierungslogik für Heading-Sprünge einzubauen

### 7.3 Siegerermittlung

- **Endzeit (`finalTimeS = elapsedS + penaltyS`)** und **Flächenanteil (Abdeckungsrate)** fließen beide in die Punktzahl ein.
- Es gibt **keinen** Mindest-Flächenanteil (z. B. 95%), den du erreichen musst.
- Ein niedrigerer Flächenanteil wirkt sich jedoch nachteilig auf die Punktzahl aus — Zeitminimierung, Flächenanteil-Maximierung und mittlere Fehlerminimierung müssen **gemeinsam optimiert** werden.
- `finalTimeS` enthält sowohl den Zeitanteil durch 10-fache Beschleunigung bei Feldaustritt als auch den Penalty durch den mittleren Fehler.

### 7.4 Bewertungsmethode

- Alle Bewertungen basieren auf **den im Server-Leaderboard erfassten Daten**.
- Wenn eine Simulation mit dem hochgeladenen RDDF ausgeführt wird, werden die Ergebnisse (`finalTimeS`, Flächenanteil usw.) automatisch im Leaderboard zusammengefasst.
- Lokal gemessene Zeiten/Flächenanteile sind nur als Referenz zu betrachten — **offizielle Ergebnisse sind die Leaderboard-Werte**.
- Es gibt kein separates Einreichungsverfahren — das beste Ergebnis deines Teams im Leaderboard zum Ablauf der Frist ist das endgültige Bewertungsergebnis.

### 7.5 Fahrt- / Verbindungs- / Teamnamen-Regeln (strikt einzuhalten)

> Bei Verstößen gegen die folgenden Regeln kann die betreffende Fahrt für ungültig erklärt werden. Bei wiederholten Verstößen droht die Disqualifikation.

- **In einer App (Simulator-Instanz) können nicht gleichzeitig zwei oder mehr Fahrten durchgeführt werden.**
  - Die nächste Fahrt darf erst gestartet werden, nachdem die vorherige vollständig abgeschlossen ist.
  - Lade keine neue RDDF hoch und starte den Simulator nicht neu, bevor die vorherige Fahrt beendet ist.
- **Parallele Fahrten mit mehreren Apps (Instanzen) sind erlaubt, aber gleichzeitige Fahrten mit derselben Kombination aus „Teamname + Karte" sind verboten.**
  - Verschiedene Karten gleichzeitig in verschiedenen Apps zu betreiben ist OK (z. B. App A mit Map 1, App B mit Map 2).
  - **Dieselbe Karte mit demselben Teamnamen in zwei Instanzen gleichzeitig zu betreiben ist verboten** — die Leaderboard-Aggregation wird korrumpiert und die Ergebnisse werden für ungültig erklärt.
- **Mehrfache Browser-Verbindungen sind verboten.**
  - Verbinde dich **nicht gleichzeitig mit mehreren Browser-Tabs oder -Fenstern** mit derselben App-Instanz.
  - Mehrfache Verbindungen können zu Fehlern bei der Leaderboard-Aggregation, Koordinaten-/Timer-Inkonsistenzen und fehlenden Ergebnissen führen.
  - Wenn du den Bildschirm teilen möchtest, nutze Screen Mirroring / Bildschirmfreigabe (keine separate Browser-Sitzung ✗).
- **Verwende bei allen Kartenfahrten denselben Teamnamen.**
  - Punktzahlen pro Karte werden **mit dem Teamnamen als Schlüssel summiert** — bereits ein einziges abweichendes Zeichen führt dazu, dass das Team als separates Team erkannt und die Punkte getrennt werden.
  - Beispiel: `Team-A`, `team-A` und `Team A` (mit Leerzeichen) werden alle als unterschiedliche Teams behandelt.
  - Lege den Teamnamen vor der ersten Fahrt fest und verwende danach bei allen Karten **exakt denselben String**.

---

## 8. Empfohlener Arbeitsablauf

```
[1] Repo klonen & IDE importieren & Build-Erfolg prüfen
        ↓
[2] rddf/1.rddf-Beispiel hochladen und Traktorverhalten im Simulator beobachten
        ↓
[3] Algorithmus entwerfen — Spurbreite, Vorgewende-Behandlung, Wendemuster festlegen
        ↓
[4] RDDF-Generator schreiben (z. B. gen_rddf.py) — eigenen Coverage-Algorithmus implementieren
        ↓
[5] Erstelltes RDDF → Checkliste §5.4 durchgehen
        ↓
   ┌─→ [6] Mit upload_rddf.sh in die Cloud hochladen
   │        ↓
   │   [7] Fahrt im Simulator ausführen → finalTime / Flächenanteil messen
   │        ↓
   │   [8] Leaderboard-Aktualisierung prüfen — bestes Ergebnis des Teams wird automatisch aktualisiert
   │        ↓
   └── [9] Algorithmus / Parameter tunen → zurück zu 4 oder 6

   * Wiederhole die Schleife [6]–[9] bis zur Deadline und verbessere bei jedem Versuch das Leaderboard-Ergebnis.
   * Es gibt kein separates "Finale einreichen" — das beste Ergebnis deines Teams
     im Leaderboard zum Ablauf der Frist (summiert über alle Karten) wird direkt bewertet.
   * Wenn du zwischen [2] und [3] feststeckst, lies §10. Zu wissen, wie der Traktor
     auf einen Pfad reagiert, macht den Algorithmus-Entwurf in [3] deutlich leichter.
```

---

## 9. FAQ

Fragen sind nach Themen gruppiert. Das `(§N)` am Ende jeder Antwort verweist auf den entsprechenden Abschnitt im Hauptteil.

- [9.1 Einreichung und Bewertung](#91-einreichung-und-bewertung)
- [9.2 Validierung und Fehler](#92-validierung-und-fehler)
- [9.3 Pfadgestaltung](#93-pfadgestaltung)
- [9.4 Betriebsregeln](#94-betriebsregeln)
- [9.5 Umgebung und Werkzeuge](#95-umgebung-und-werkzeuge)

---

### 9.1 Einreichung und Bewertung

> **F1.** Wird eine RDDF-Datei für alle Karten eingereicht oder für jede Karte separat?

**Für jede Karte separat hochladen.** Der Wettbewerb verwendet drei öffentliche Karten M1, M2 und M3. Für jede Karte wird die RDDF mit der jeweiligen mapId hochgeladen. Punktzahlen pro Karte werden mit dem **Teamnamen** als Schlüssel summiert. (§4.5, §7.5)

---

> **F2.** Muss der Flächenanteil 100 % erreichen, um zu gewinnen?

**Nein — es gibt keinen Mindest-Flächenanteil.** Da der Flächenanteil jedoch proportional in die Punktzahl einfließt, ist ein Team mit höherem Flächenanteil bei gleicher Zeit im Vorteil. Die **Balance zwischen Zeit und Flächenanteil** ist der Kern der Strategie.

---

> **F3.** Gibt es ein separates „finales Einreichungs"-Verfahren?

**Nein.** Das beste Ergebnis deines Teams im Leaderboard zum Ablauf der Deadline ist direkt das Bewertungsergebnis. Lade bis zur Deadline weiter hoch und fahre Runden, um dein Ergebnis zu verbessern. (§7.4)

---

### 9.2 Validierung und Fehler

> **F4.** Ich habe die RDDF hochgeladen, aber im Simulator passiert nichts.

Prüfe zwei mögliche Ursachen:

| Ursache | Prüfung / Maßnahme |
|---------|-------------------|
| **Validierungsfehler** | Prüfe, ob ein „RDDF Validation Error"-Dialog erschienen ist → RDDF entsprechend dem Ablehnungsgrund korrigieren und erneut hochladen (§4.4) |
| **Vorheriger Track noch aktiv** | Prüfe, ob die neue RDDF zwar in der pending-Liste erscheint, der **apply**-Schritt jedoch ausgeblieben ist |

---

> **F5.** Warum prüft die Dateivalidierung den Karten-StartPoint nicht?

Der App-Loader kennt keine autoritative Live-Spawnposition. Die Ausrichtung wird
deshalb vor der Bewegungsfreigabe durch die Simulator-/Sitzungsprüfung gegen
Waypoint 0 geprüft. (§4.4)

---

> **F6.** Darf ich leicht über 7 km/h gehen?

**Nein.** Werte über 7,0 km/h werden ausdrücklich abgelehnt und nicht still
begrenzt. Nicht-null-Werte unter 2,05 km/h werden mit einer Warnung akzeptiert;
die Maschine kann zur Laufzeit ihre Kriechgrenze anwenden. (§4.2, §4.4)

---

### 9.3 Pfadgestaltung

> **F7.** Ist eine Route mit viel Overlap vorteilhaft?

**Nein, sie ist nachteilig.** Overlap selbst führt zu keinem Punktabzug, verlängert aber die Fahrzeit — das ist im Zeitwettbewerb ein Nachteil. Entwirf eine möglichst effiziente Coverage-Route.

---

> **F8.** Ist Rückwärtsfahrt beim Wenden absolut verboten?

**Nur Rückwärtsfahrt innerhalb der gepflügten Fläche (Pflug abgesenkt)** löst den Penalty für den Reset ungepflügter Bereiche aus.

- OK: Rückwärtsfahrt im Vorgewende-Bereich (`implementFlag=0`) — kein Penalty für ungepflügte Bereiche
- NG: Trotzdem in der Regel ineffizient wegen Zeitverlust und schwieriger Heading-Umkehr-Behandlung

---

> **F9.** Eine große U-Kurve außerhalb des Felds ist praktisch — wie groß ist die Penalty?

**Solange der Traktor außerhalb der Feldgrenze ist, läuft der Timer 10-mal schneller.** Beispiel: 3 Sekunden Wenden außerhalb des Vorgewindes ergeben 30 Sekunden Zeitzuschlag. Schließe U-turns so weit wie möglich innerhalb des Felds ab. (§7.2)

---

> **F10.** Wie groß ist der Einfluss der mittleren Fehler-Penalty?

Die Formel lautet `penaltyS = deviationM × 1000` Sekunden. Konkrete Beispiele:

| Mittlerer CTE | Penalty |
|--------------:|--------:|
| 0,1 m | 100 s |
| 0,5 m | 500 s |
| 1,0 m | 1000 s |

→ Lässt sich durch höhere **Waypoint-Dichte in Kurvenabschnitten** und **Minimierung von Heading-Sprüngen** reduzieren. (§7.2)

---

### 9.4 Betriebsregeln

> **F11.** Darf ich auf einem Gerät zwei Karten gleichzeitig betreiben?

**Ja — aber die Kombination „gleicher Teamname + gleiche Karte" ist verboten.**

- OK: App A mit M1, App B mit M2 gleichzeitig ausführen
- NG: Zwei Instanzen mit demselben Teamnamen laufen gleichzeitig auf M1
- NG: Mehrere Browser-Tabs, die sich mit derselben App-Instanz verbinden

(§7.5)

---

> **F12.** Darf ich den Teamnamen zwischendurch ändern?

**Nicht empfohlen.** Da der Teamname der Schlüssel für die kartenübergreifende Punktesummierung ist, führt bereits **ein einziges abweichendes Zeichen** dazu, dass das Team als separates Team erkannt und die Punkte getrennt werden. Lege den Teamnamen vor der ersten Fahrt fest und verwende denselben exakten String bei allen Karten und allen Fahrten. (§7.5)

---

### 9.5 Umgebung und Werkzeuge

> **F13.** Darf ich das RDDF-Erstellungswerkzeug und die Programmiersprache frei wählen?

**Ja — keine Einschränkungen bei Werkzeug oder Sprache.** Python, MATLAB, eigene Algorithmen — alles ist erlaubt. Das Endergebnis muss lediglich eine `.rddf`-Datei sein, die das in diesem Dokument beschriebene Format erfüllt. (§4.1)

---

> **F14.** Darf ich den Code (C++-App, Pfadverfolgungs-Tracker usw.) modifizieren?

**Ja.** Teilnehmer dürfen Autonomielogik und Parameter ändern. Die offizielle
Bewertung verwendet den genehmigten FIF-/App-Build des Teilnehmers zusammen mit
dem eingereichten RDDF gemäß den aktuellen Veranstalterregeln. Änderungen
müssen über den offiziellen FIF-Workflow (§3.2) paketiert und installiert sein;
ein lokaler oder lose kopierter Binär-Build gilt nicht automatisch als zulässig.

Das **Lesen** des Verfolgungscodes ist dagegen ausdrücklich empfohlen. Wenn du weißt, welchen Pfaden der Traktor gut folgt und bei welchen sein Fehler wächst, verbessert das unmittelbar die Qualität deines RDDF. §10 erklärt den Aufbau und wie man ihn liest.

---

## 10. Fortgeschritten: Den Pfadverfolgungs-Code verstehen (optional)

> Der Tracker ist Teil der Teilnehmer-App. Änderungen können nur dann offiziell
> bewertet werden, wenn sie gemäß den Veranstalterregeln im genehmigten FIF des
> Teilnehmers enthalten sind (§9.5 F14). Das Lesen hilft außerdem, zuverlässig
> verfolgbare RDDF-Geometrie zu verstehen.

### 10.1 Wo der Code liegt

Die Pfadverfolgung ist an einer Stelle gebündelt: `master_of_plow_app/src-gen/AppMain/tracking/`.

```
AppMain/tracking/
├── TrackerTypes.hpp      Koordinaten-/Vorzeichenkonventionen + Datentypen  ← hier anfangen
├── IPathTracker.hpp      Der Vertrag, den ein Verfolgungsalgorithmus erfüllen muss
├── PathTrackerBase.hpp    Basisklasse, die den Boilerplate-Teil bereits umsetzt
├── SpeedController.hpp    Gang/Gas/Bremse (für alle Algorithmen gemeinsam)
├── TrackerFactory.*       Name → Implementierung
├── TrackingLoop.*         Regelschleife (GPS-Umrechnung, Befehle, Telemetrie)
└── impl/
    ├── PurePursuitTracker.*  Standard-Tracker (der im Wettbewerb tatsächlich fährt)
    └── StanleyTracker.*      Ein zweites, ausgearbeitetes Beispiel
```

Alle Kommentare sind **auf Koreanisch und Englisch** verfasst. Beginne oben in
`TrackerTypes.hpp` — ohne die Konventionen liest sich der Rest falsch.

### 10.2 Koordinaten und Vorzeichen — das Wichtigste zuerst

| Aspekt | Konvention |
|--------|------------|
| Position | ENU-Meter `(x, z)`, **+x = Osten, +z = Süden**. Ursprung = Waypoint 0 des RDDF |
| Kurs | `atan2(z, x)` in Radiant. 0 = nach Osten, +π/2 = nach Süden |
| Lenkung | Positiv = **Linkskurve**. Das System ist linkshändig, eine Linkskurve *verringert* also den Kurs |
| Geschwindigkeit | Waypoint-Wert in km/h **mit Vorzeichen** (negativ = Rückwärts); Messwert in m/s **ohne Vorzeichen** |

Den Kompasskurs des GPS (0 = Norden, im Uhrzeigersinn) rechnet die Regelschleife
in dieses System um.

### 10.3 Ein Regelzyklus

`TrackingLoop` wiederholt alle 100 ms diese Reihenfolge (bei höherem
Simulations-Geschwindigkeitsfaktor entsprechend kürzer):

```
isReady()  →  sense()  →  tracker->update()  →  act()  →  report()
  bereit?     GPS→ENU     Verfolgungsalgor.    Befehle    Telemetrie
```

`sense()` und `act()` sind die Absprache mit dem Simulator und für jeden
Algorithmus identisch. Das Einzige, was sich ändert, ist `update()`.

### 10.4 Wie der Standard-Tracker den Lenkwinkel bestimmt

Trotz des Namens ist es eine **gewichtete Summe aus fünf Termen**; die reine
Pure-Pursuit-Geometrie trägt nur 45 % bei.

| Term | Aufgabe |
|------|---------|
| Krümmungs-Vorsteuerung | Lenkung, die die Krümmung des Pfades selbst verlangt (fehlerunabhängig) |
| Kursfehler | Richtet das Fahrzeug an der Pfadtangente aus |
| Querabweichung | Zieht das Fahrzeug zurück auf den Pfad. Auf Bahnen gemessen an der **um 1,5 s vorausberechneten Position**, in Wenden an der aktuellen Position |
| Pure Pursuit | Kreisbogen zum Vorausschaupunkt — zuständig für Laufruhe (Gewicht 0,45) |
| Gierraten-Dämpfung | Unterdrückt Schwingungen, wenn die Terme oben überreagieren |

### 10.5 ★ Eigenschaften mit direktem Einfluss auf den RDDF-Entwurf

Das ist der praktischste Teil dieses Abschnitts.

| Eigenschaft des Trackers | So schreibst du dein RDDF |
|--------------------------|---------------------------|
| Vorausschaudistanz **5,5–8 m** (skaliert mit der Geschwindigkeit) | Kurven mit engerem Radius werden innen abgeschnitten. Scharfe Wenden entschärfen oder Waypoints dichter setzen |
| Krümmung als gewichteter Mittelwert über **5 aufeinanderfolgende Punkte** um den nächstgelegenen geschätzt | Zu weite Waypoint-Abstände verschlechtern die Schätzung, sodass Bögen untersteuert werden (§4.4 Regel 4) |
| Das Anbaugerät läuft **3 m hinter** dem Fahrzeug | `implementFlag`-Wechsel greifen, wenn das Gerät die Stelle passiert — plane an Bahnanfang und -ende etwa 3 m Reserve ein |
| Verfolgung startet erst **innerhalb von 3 m um Waypoint 0** | Setze den ersten Waypoint auf Punkt A der Karte (§4.4) |
| Ein Wechsel vorwärts↔rückwärts **bremst bis zum Stillstand** | Jeder Richtungswechsel kostet einen kompletten Stopp plus Wiederanfahren. Rückwärtsfahren macht gepflügten Boden zudem ungültig (§7.2) — vorwärtsbasierte Wenden gewinnen |
| Abschluss = **innerhalb 1 m** um den letzten Waypoint + Fortschritts-Gate | Mache den letzten Waypoint zu einem eindeutigen Endpunkt |
| Der Fortschritt läuft nie zurück, auch wenn sich der Pfad selbst schneidet | Muster mit übersprungenen Bahnen sind unbedenklich |

### 10.6 Verhalten lokal beobachten

Beim lokalen Ausführen der App entstehen zwei Diagnose-Ausgaben.

- **`/tmp/pp_trace.csv`** — eine Zeile pro Regelzyklus: Fahrzeugpose, Lenkung,
  Querabweichung und **jeder der fünf Lenkterme einzeln**, sodass sofort sichtbar
  ist, welcher davon ausbricht. Nach der Fahrt kannst du Pfad und tatsächliche
  Trajektorie in Python oder einer Tabellenkalkulation überlagern.
- **`PP_LOG ...`** (stderr) — eine Zusammenfassungszeile pro Zyklus, zum
  Mitverfolgen im Terminal.

Wenn sich `cte` in einer Kurve dauerhaft zu einer Seite aufbaut, übersteigt die
Krümmung dieser Kurve die physikalische Grenze des Traktors. Bevor du an einem
Gain drehst, **entschärfe zuerst die Krümmung im RDDF**.

### 10.7 Einen anderen Algorithmus ausprobieren

Neben dem Standard-Tracker ist eine Stanley-Beispielimplementierung enthalten.
Eine Umgebungsvariable schaltet um:

```bash
PATH_TRACKER=stanley   # Standard ist pure_pursuit
```

Einen eigenen Algorithmus anzubinden, sind drei Schritte:

1. Erstelle in `tracking/impl/` eine Klasse, die von `PathTrackerBase` erbt, und
   implementiere `update()` und `isTrackComplete()` (am einfachsten, indem du
   `StanleyTracker` kopierst)
2. Ergänze eine Zeile im Registry in `TrackerFactory.cpp`
3. Starte mit `PATH_TRACKER=<Name>`

`master_of_plow_app/tests/PathTrackerContractTest.cpp` prüft **jeden registrierten
Tracker** automatisch — nach Schritt 2 ist deine Implementierung also mit
abgedeckt. Wo googletest installiert ist, wird zusätzlich das Target
`master_of_plow_app_tests` gebaut.

Änderungen müssen die lokalen Tests bestehen und im genehmigten FIF enthalten
sein. Das direkte Ersetzen einer Binärdatei ist kein offizieller Lauf.

---

## 11. Hilfe und Support

- **Dringende Anfragen**: Direkt an das Organisationsteam am Stand vor Ort

Viel Erfolg.
