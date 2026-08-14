# SeamOS Hackathon 2026: The Master of Plowing — Teilnehmer-Leitfaden

> Dieses Dokument ist ein praxisorientierter Leitfaden für Teilnehmer des SeamOS Hackathon 2026.
> Hintergrundinformationen zum Wettbewerb und Bewertungskriterien findest du im separaten Konzeptdokument. Dieses Dokument behandelt den vollständigen Ablauf: **Repository-Setup → RDDF erstellen → im App anwenden**.

---

## 1. Wettbewerbsübersicht

- **Mission**: Steuere einen virtuellen Traktor und pflüge die Fläche **„Alpha"** in **kürzester Zeit + maximaler Flächenabdeckung**
- **Harte Einschränkung**: Maximale Fahrgeschwindigkeit **7 km/h** (Werte darüber werden abgelehnt, nicht still begrenzt)
- **Bewertung**: Eine einzige mit REC aufgezeichnete CSV der Fahrt ergibt eine Punktzahl **von 100**.
  - Sechs Posten — **Coverage 45 · Duplicate 17 · Time 16 · Retread 9 · Outside 7 · Outside Time 6**
  - Der Erfüllungsgrad (0–1) jedes Postens wird **kubiert** und dann mit der Punktzahl multipliziert; Punkte kommen erst nahe der Bestmarke schnell dazu
  - Beispiel: 50% Abdeckung ergeben **5,6** der 45 Coverage-Punkte (0,5³ = 12,5%)
  - Es gibt **keinen harten Cutoff** (z. B. 95%) — auch bei niedriger Abdeckung wird eine Punktzahl gebildet
- **INVALID**: manuelles Fahren, Tastatureingaben oder ein Anbaugerätewechsel während der Fahrt bedeuten **gar keine Wertung**

> **Kernaussage**: Was Teilnehmer erstellen und einreichen, ist eine **RDDF (Route Definition Data File)**.
> Der Traktor-Steuerungsalgorithmus selbst wird vom bereitgestellten Skeleton übernommen — Teilnehmer formulieren die **optimale Route zum Pflügen des Felds** als RDDF und reichen diese ein.

---

## 2. Voraussetzungen

| Komponente | Hinweis |
|------|------|
| **SeamOS IDE (FeatureDesigner)** | Distributionsversion der Organisatoren verwenden |
| **Claude Code** | Aktuelle Version von der offiziellen Website installieren |
| **SeamOS Everywhere** | Distributionsversion der Organisatoren verwenden |
| **SeamOS World (Emulator)** | Umgebung, in der das RDDF tatsächlich gefahren und geprüft wird. Lokale Installation oder `seamosworld.seamos.io` |


---

## 3. Repository-Struktur

```
master_of_plow/
├── com.bosch.fsp.master_of_plow/            # FSP (Feature Spec Project)
├── com.bosch.fsp.master_of_plow.gen/        # Aus dem FSP erzeugte Artefakte
├── com.bosch.fsp.master_of_plow.gen.tests/  # Erzeugte Test-Artefakte
├── master_of_plow_app/                      # App-Hauptteil (C++-Code, Konfiguration)
│   ├── src-gen/AppMain/tracking/            # Pfadverfolgungs-Skelett (§10)
│   └── tests/                               # Lokale Testmodule
├── master_of_plow_CPP_SDK/                  # SeamOS C++ SDK (bereitgestellt)
├── customui-src/                            # Dashboard-Quellcode (React + Vite)
├── rddf/                                    # RDDF-Bereich (Erstellen & Validieren durch Teilnehmer)
├── docs/                                    # Ablaufsteuerung und FIF-Validierung
├── distribution/                            # Distributionsartefakte
├── seamos-assets/                           # Marketplace-Bilder und weitere Assets
└── HACKATHON_GUIDE.md                       # Dieses Dokument (mit en · de · th Übersetzungen)
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

> Der Parser akzeptiert auch eine **7-Spalten-Form** ohne die führenden `lineNo` und
> `index` (`RddfParser.cpp`). Dieser Leitfaden und die Beispieldateien nutzen 9 Spalten,
> was wir empfehlen.

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
- `-7.0 ≤ speed < 0` — Rückwärtsfahrt. Eine eigene Rückwärts-Penalty gibt es nicht, aber die Zeit fürs Anhalten und Wiederbeschleunigen sowie die Doppelarbeit kosten Punkte (§7.4); Beträge unter `2,05 km/h` erzeugen eine Warnung
- Wenn das Vorzeichen der Geschwindigkeit zwischen benachbarten Waypoints wechselt, liegt ein **Gangwechselpunkt** vor — die Waypoints müssen so angeordnet sein, dass die Fahrtrichtung (Heading) umgekehrt wird

### 4.3 implementFlag

- Beim Pflügen auf einer Spur: `1`
- Beim Wenden im Vorgewende (headland) am Feldrand: `0`
- **Rückwärtsabschnitte immer `0`** — beim Rückwärtsfahren wird nicht gepflügt.
- Auch beim Wenden im Vorgewende zählt ein Anbaugerät über bereits bearbeitetem Boden als `Retread` (§7.2)

### 4.4 Automatische Validierung

RDDF-Dateien werden sofort nach Empfang aus der Cloud automatisch validiert. Formatfehler und Verstöße gegen Sicherheitsobergrenzen werden **nicht gespeichert und abgelehnt**; physikalische Nachführgrenzen werden unverändert geladen und als Warnung gemeldet. Die Validierungslogik ist in `RddfValidator.cpp` definiert.

| Prüfung | Inhalt | Bei Verstoß |
|---------|--------|-------------|
| **Leere Datei** (Rule 4) | 0 Waypoints | abgelehnt |
| **Wertgültigkeit** | `lat`, `lon` und `speed` müssen endlich sein (kein NaN, kein inf) | Ablehnung mit Angabe des Waypoint-Index |
| **implementFlag** | Nur `0` oder `1` zulässig | Ablehnung mit tatsächlichem Wert und Waypoint-Index |
| **Geschwindigkeitsobergrenze** | `\|speed\| ≤ 7,0 km/h` | Oberhalb der Grenze ablehnen |
| **Geschwindigkeitsuntergrenze** | Nicht-null-Betrag unter `2,05 km/h` | Unverändert laden und warnen |
| **Waypoint-Abstand** (Rule 3) | Abstand benachbarter Punkte **mindestens 0,05 m und höchstens 5,0 m** | Ablehnung mit dem betroffenen Paar und dem tatsächlichen Abstand |
| **Physikalische Krümmung** (Rule 6) | Krümmung aus der Headingänderung über drei aufeinanderfolgende Punkte, verglichen mit dem Mindestwendekreis des Fahrzeugs (ca. **4,35 m**) | Unverändert laden und vor der Nachführgrenze warnen |

> Die Konstanten stehen in `RddfValidator.hpp`: `MIN_WAYPOINT_SPACING_M = 0.05`,
> `MAX_WAYPOINT_SPACING_M = 5.0`, `MAX_MACHINE_SPEED_KMH = 7.0`,
> `MIN_MACHINE_SPEED_KMH = 2.05`; der Mindestwendekreis wird direkt aus den
> Tracker-Konstanten als `WHEELBASE_M / tan(WHEEL_MAX_RAD)` berechnet.
> Die Ablehnung **stoppt beim ersten Verstoß**, es wird also immer nur ein Grund gemeldet.

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

Der Wettbewerb verwendet **drei Karten (M1, M2, M3)**. Alle drei teilen sich denselben
**GPS-Ursprung — `37.5665, 126.978`**.

| Karte | id | Name | Kartengröße | Befahrbar (driveArea) | Gewertet (workArea) |
|-------|----|------|-------------|-----------------------|---------------------|
| **M1** | `agri-1-plain` | Open Fields | 88 × 80 m | 3.187 m² | **1.499 m²** |
| **M2** | `agri-2-sloped` | Sloped Acres | 80 × 80 m | 2.132 m² | **1.947 m²** |
| **M3** | `agri-3-patch` | Patchwork Plots | 72 × 80 m | 1.575 m² | **1.356 m²** |

Gewertet wird nur das Innere von `workArea`. `driveArea` ist die Fahrgrenze; dahinter liegt ein
1,4 m tiefer Graben — **ist ein Rad erst drin, kommt der Traktor nicht mehr allein heraus.**
Alle drei Karten sind vollständig eben.

#### Startposition — richte Waypoint 0 hierauf aus

| Karte | Start (lat, lon) | Heading | Welt (x, z) |
|-------|------------------|---------|-------------|
| **M1** | `37.5663023, 126.9780595` | `32,3°` | 5,25, 21,85 |
| **M2** | `37.5665036, 126.9783060` | `-48,1°` | 27,0, -0,4 |
| **M3** | `37.5662952, 126.9782923` | `-17,5°` | 25,79, 22,64 |

- Heading ist **0° = Norden, im Uhrzeigersinn** (`forward = (sin h, -cos h)`).
- Die Startkoordinate ist der **Aufstandspunkt des rechten Hinterrads**, nicht die Fahrzeugmitte.
  Auch bei einer anderen Traktorklasse steht das rechte Hinterrad exakt hier.
- Die Fahrfreigabe kommt erst, wenn die tatsächliche Fahrzeuglage zu Waypoint 0 passt (§4.4).

#### Umrechnung Weltkoordinaten in lat/lon

Die Polygone im Kartendokument stehen in Welt-`[x, z]` in Metern (**+x = Osten / -z = Norden**).
Ein RDDF braucht lat/lon, also so umrechnen:

```
lat = 37.5665 - z / 110540
lon = 126.978 + x / (111320 x cos(37.5665°))
```

> Mit `111320` für den Breitengrad verschieben sich alle Bewertungszellen um 0,7%. Für den
> Breitengrad **muss** **`110540`** verwendet werden. Das Leaderboard invertiert dieselbe Formel
> auf 0,5-m-Bewertungszellen.

#### M1 · Open Fields

![M1 Open Fields](docs/maps/map1_polygon.png)

Das einfachste rechteckige Feld — ideal, um einen Boustrophedon-Algorithmus schnell zu prüfen.

#### M2 · Sloped Acres

![M2 Sloped Acres](docs/maps/map2_polygon.png)

Ein komplexes Polygon mit 37 Eckpunkten. Trotz des Namens gibt es keine Steigung. Das lange
Vorgewende macht die U-turn-Behandlung heikel.

#### M3 · Patchwork Plots

![M3 Patchwork Plots](docs/maps/map3_polygon.png)

Ein unregelmäßiges Feld mit konkaven und konvexen Kanten (54 Eckpunkte). Ein einfaches
Boustrophedon lässt viel ungepflügt — hier braucht es Polygon-Clipping und Behandlung je Teilfläche.

#### Detaillierte Spezifikationsdokumente

Vollständige Polygonkoordinaten, Bodeneigenschaften und Traktordaten stehen in eigenen Dokumenten.

| Dokument | Inhalt |
|----------|--------|
| [maps.md](docs/hackathon-2026/maps.md) | Kartengrößen, GPS-Ursprung, **vollständige `workArea`/`driveArea`-Polygonkoordinaten**, Bewertungsgrundlage der Bodenbearbeitung (0,2-m-Zellen, Abschluss 0,999) |
| [terrain.md](docs/hackathon-2026/terrain.md) | Reibung, Traktionsgrenzen und Formeln des Bearbeitungswiderstands mit Werten |
| [tractor-specs.md](docs/hackathon-2026/tractor-specs.md) | Physikalische Daten der drei Traktoren (klein · mittel · groß) |
| [rddf-format.md](docs/hackathon-2026/rddf-format.md) | Spezifikation des RDDF-Formats |
| [signal-flow.md](docs/hackathon-2026/signal-flow.md) | Wie deine App Sensorsignale empfängt und Befehle sendet |
| [system-requirements.md](docs/hackathon-2026/system-requirements.md) | Anforderungen an den Teilnehmer-PC |

---

## 5. RDDF-Erstellungs-Workflow

### 5.1 Verzeichnisstruktur

```
master_of_plow/rddf/
├── 1.rddf                  # Route für Feld 1
├── 2.rddf                  # Route für Feld 2
├── 3.rddf                  # Route für Feld 3
├── upload_rddf.sh          # Cloud-Upload-Skript
├── how-to-upload-rddf.md   # Hinweise zu Upload-Befehlen
└── README.md               # Dateikonventionen und Validierungsverhalten
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

#### Beispiel: Skip-Lane-Boustrophedon + Halbkreis-U-turn

Das Feld wird in parallele Spuren im SWATH-Abstand (Pflugbreite) aufgeteilt; am Spurende
erfolgt der Übergang zur nächsten Spur über einen **vorwärts gefahrenen Halbkreis-U-turn**.
Ohne Rückwärtsfahrt entfällt die Zeit fürs Anhalten und Wiederbeschleunigen.

**Eine einfache Hin- und Rückfahrt in die Nachbarspur ist nicht möglich.** Dieser Halbkreis
hätte den Radius `SWATH / 2`, bei SWATH = 4 m also 2 m — deutlich unter dem Mindestwendekreis
von **4,35 m**. Die Lenkung geht auf Anschlag und das Fahrzeug läuft trotzdem aus der Kurve.

Deshalb werden Spuren **übersprungen**. Wendet man in eine Spur, die `d` Spuren entfernt liegt,
beträgt der Radius `d × SWATH / 2`, und dieser Wert muss bei jeder Wende mindestens 4,35 m sein.

Befährt man die **vordere und die hintere Hälfte der Spuren abwechselnd**, ist das immer erfüllt.
Bei 7 Spuren lautet die Reihenfolge `0, 4, 1, 5, 2, 6, 3`; die Abstände sind 4, 3, 4, 3, 4, 3,
sodass selbst die engste Wende einen Radius von `3 × 4 / 2 = 6 m` hat.

```
Spur:      0    1    2    3    4    5    6
Reihenfolge: 1  3    5    7    2    4    6
           └── vordere Hälfte ──┘└ hintere ┘

  ↑ Spur 0 ─────────╮
                    │  Radius = (Spurabstand) × SWATH / 2  ≥ 4,35 m
  ↓ Spur 4 ─────────╯
```

Der kleinste Abstand entspricht `Spurenzahl der vorderen Hälfte − 1`. Das Muster setzt also
voraus, dass das Feld breiter ist als `SWATH × (2 × 4,35 / SWATH + 1)`. Bei einem schmaleren
Feld ist ein Halbkreis-U-turn gar nicht nutzbar; dann kommen nur eine Omega-Wende über die
Feldgrenze hinaus (→ Outside-Abzug) oder eine Dreipunktwende (→ Rückwärtsabschnitt) infrage.

#### Minimales funktionsfähiges Beispielskript (`gen_rddf.py`)

```python
"""
gen_rddf.py - Beispielgenerator fuer ein RDDF im Skip-Lane-Boustrophedon
Als Ausgangspunkt gedacht - baue daraus deinen eigenen Algorithmus.
"""

import math

# --- Feldursprung und Umrechnung in lat/lon (alle drei Wettbewerbskarten teilen ihn) ---
LAT0, LON0 = 37.5665, 126.978
M_PER_DEG_LAT = 110540.0
M_PER_DEG_LON = 111320.0 * math.cos(math.radians(LAT0))

def to_latlon(x_m, y_m):
    return (LAT0 + y_m / M_PER_DEG_LAT,
            LON0 + x_m / M_PER_DEG_LON)

# --- Physikalische Grenzen des Fahrzeugs (gleiche Werte wie RddfValidator) ---
MIN_TURN_R  = 4.35     # WHEELBASE_M 2.05 / tan(WHEEL_MAX_RAD 0.44)
MAX_SPACING = 5.0      # max. Abstand benachbarter Waypoints (darueber wird die Datei abgelehnt)

# --- Parameter ---
FIELD_W = 30.0     # Feldbreite, Ost-West (m)
FIELD_H = 50.0     # Feldlaenge, Nord-Sued (m)
SWATH   = 4.0      # Spurabstand = Pflugbreite (m)
STEP    = 1.0      # Waypoint-Abstand (m) - muss <= MAX_SPACING sein
V_LANE  = 3.0      # Spurgeschwindigkeit (km/h)
V_TURN  = 2.5      # Wendegeschwindigkeit (km/h) - mindestens die Kriechgrenze 2.05
N_ARC   = 16       # Segmente je U-turn-Halbkreis

assert STEP <= MAX_SPACING

n_lanes = int((FIELD_W - SWATH) // SWATH) + 1

# Direkt in die Nachbarspur zu wenden ergibt Radius SWATH/2 - unter dem Minimum.
# Vordere und hintere Haelfte abwechselnd zu befahren haelt jeden Abstand bei
# mindestens (half - 1) Spuren.
half = (n_lanes + 1) // 2
order = []
for i in range(half):
    order.append(i)
    if i + half < n_lanes:
        order.append(i + half)

# Kleinster genutzter Wenderadius und wie weit die Halbkreise ueberstehen.
MIN_R_USED = (half - 1) * SWATH / 2.0
HEADLAND   = half * SWATH / 2.0
assert MIN_R_USED >= MIN_TURN_R, (
    f"field too narrow: turn radius {MIN_R_USED:.2f} m < {MIN_TURN_R} m")

# Vorgewende an beiden Enden freilassen, damit die Halbkreise im Feld bleiben.
Y_LO, Y_HI = HEADLAND, FIELD_H - HEADLAND

waypoints = []  # (x, y, speed, implementFlag)
lane_x = lambda k: SWATH / 2.0 + k * SWATH

for n, k in enumerate(order):
    x = lane_x(k)
    going_up = (n % 2 == 0)

    # (1) Gerade Spur - Anbaugeraet ABGESENKT
    y0, y1 = (Y_LO, Y_HI) if going_up else (Y_HI, Y_LO)
    n_steps = max(2, int(abs(y1 - y0) / STEP) + 1)
    for i in range(n_steps):
        t = i / (n_steps - 1)
        waypoints.append((x, y0 + (y1 - y0) * t, V_LANE, 1))

    # (2) Halbkreis-U-turn in die naechste Spur - Anbaugeraet ANGEHOBEN
    if n == len(order) - 1:
        break
    x_next = lane_x(order[n + 1])
    r = abs(x_next - x) / 2.0
    assert r >= MIN_TURN_R, f"turn radius {r:.2f} m < {MIN_TURN_R} m"
    cx, cy = (x + x_next) / 2.0, y1
    th0 = math.pi if x_next > x else 0.0
    th1 = 0.0 if x_next > x else math.pi
    sign = 1.0 if going_up else -1.0            # oben nach oben ausbeulen, unten nach unten
    # Der letzte Punkt (j = N_ARC) faellt mit dem ersten Punkt der naechsten Spur zusammen.
    # Dieselbe Koordinate zweimal ergibt Abstand 0 und wird abgelehnt.
    for j in range(1, N_ARC):
        theta = th0 + (th1 - th0) * j / N_ARC
        waypoints.append((cx + r * math.cos(theta),
                          cy + sign * r * math.sin(theta),
                          V_TURN, 0))

# --- RDDF-Datei schreiben (tab-getrennt, 9 Spalten) ---
with open("alpha.rddf", "w") as f:
    for idx, (x, y, v, flag) in enumerate(waypoints, 1):
        lat, lon = to_latlon(x, y)
        f.write(f"{idx}\t{idx}\t{lat:.7f}\t{lon:.7f}\t0.0\t0.0\t0.0\t{v:.2f}\t{flag}\n")

print(f"{len(waypoints)} waypoints, {n_lanes} lanes, "
      f"min turn R {MIN_R_USED:.1f} m, headland {HEADLAND:.1f} m")
```

#### Punkte zur Weiterentwicklung mit eigenem Algorithmus

Das obige Skript behandelt nur **rechteckige Felder mit gleichmäßigen Spuren** und lässt die
Vorgewende-Streifen (jeweils `HEADLAND` tief) völlig ungepflügt. Im tatsächlichen Wettbewerb
müssen folgende Punkte durch eigene Algorithmen ersetzt werden:

- **Polygon-Verarbeitung der Feldgrenze** — Spurlängen an die tatsächliche Feldform (kein Rechteck) anpassen
- **Vorgewende füllen** — die vom Beispiel übrig gelassenen Streifen in einem abschließenden separaten Durchgang bearbeiten. Sonst gehen sie direkt von der Coverage ab
- **Wenden innerhalb des Felds abschließen** — ein Halbkreis über die Grenze hinaus kostet **Outside** für die Fläche und **Outside Time** für die Zeit (§7.2)
- **Doppelarbeit minimieren** — eine bereits fertige Zelle erneut zu überfahren zählt als **Duplicate**, Räder auf bearbeitetem Boden als **Retread** (§7.2)
- **Optimierung von Spurabstand / Wenderadius** — ein kleinerer Spurabstand verkürzt die Übergänge, solange jede Wende den Mindestradius von **4,35 m** einhält
- **Start- und Endpunkt** — Entwurf der Einfahrtroute vom Spawn-Punkt der Karte zur ersten Spur
- **Minimierung ungepflügter Bereiche** — kleine ungepflügte Bereiche an Ecken und Rändern separat behandeln

### 5.4 Validierungs-Checkliste

Prüfe folgende Punkte, bevor du das RDDF hochlädst.

- [ ] Alle Zeilen konsistent mit 9 Spalten und **Tabulator-Trennzeichen**?
- [ ] `lineNo` beginnt bei 1 und steigt lückenlos an?
- [ ] Startpose von Karte/Sitzung vor der Bewegungsfreigabe an Waypoint 0 ausgerichtet? (§4.4)
- [ ] Hat jeder `speed`-Wert einen Absolutwert von **höchstens 7,0 km/h** (unter `2,05 km/h` erfolgt eine Warnung)?
- [ ] Liegt der Abstand benachbarter Waypoints im Bereich **0,05 m ~ 5,0 m**? — außerhalb dieses Bereichs wird die Datei abgelehnt (§4.4 Regel 3)
- [ ] Sind auch gerade Abschnitte mit **höchstens 5 m** Abstand besetzt? — eine lange Gerade mit nur Start- und Endpunkt wird abgelehnt
- [ ] Sind Kurvenabschnitte dicht genug besetzt? — je größer die Krümmung, desto feiner die Unterteilung
- [ ] Bei Rückwärts → Vorwärts-Wechsel: **Heading umgekehrt**? (Drehung auf der Stelle nicht möglich)
- [ ] `implementFlag` aller Rückwärtsabschnitte ist `0`?
- [ ] Ist jeder Wenderadius mindestens **4,35 m**? — Engeres kann der Traktor physikalisch nicht fahren (§4.4)
- [ ] Verlässt der U-turn-Pfad die Feldgrenze nicht? (Abzüge bei Outside · Outside Time, §7.2)

---

## 6. RDDF in der App anwenden

### 6.1 Cloud-Upload

Die Wettbewerbsbewertung basiert auf dem **in der Cloud hochgeladenen RDDF**. Lade es mit der von den Organisatoren bereitgestellten Umgebungsdatei (Postman environment JSON) sowie den Werten für `FEU_ID` und `FEATURE_ID` hoch.

> `--env` ist erforderlich. Die Umgebungsdatei enthält die Schlüssel `tokenUrl`, `baseUrl`,
> `cp_client_id`, `cp_client_secret`, `feature_id` und `feu_id`; `--feature-id` und `--feu-id` sind
> optionale Overrides für die Werte aus dieser Datei. `jq` und `curl` müssen installiert sein.

```bash
cd master_of_plow/rddf

# Ausführungsberechtigung erteilen (einmalig)
chmod +x ./upload_rddf.sh

# Hochladen
./upload_rddf.sh \
  --env ./participant-env.json \
  -f ./1.rddf \
  --feu-id <YOUR_FEU_ID> \
  --feature-id <YOUR_FEATURE_ID>
```

Beispiel einer erfolgreichen Ausgabe:

```
Uploaded ./1.rddf (HTTP 200).
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
|-------|------|
| Startposition | Der **Spawn-Punkt der Karte**. Die Fahrfreigabe kommt erst, wenn Waypoint 0 deines RDDF dort liegt (§4.4) |
| Zielfelder | **M1 · M2 · M3** (§4.5) |
| Zielvorgabe | **Kein harter Cutoff** — auch bei niedriger Abdeckung wird gewertet |
| Höchstgeschwindigkeit | **7 km/h** |
| Anbaugerät | **Genau eines** pro Fahrt — ein Wechsel während der Aufzeichnung macht die Fahrt ungültig |

### 7.2 Bewertung (100 Punkte)

Eine **einzige, mit REC aufgezeichnete CSV** ergibt die Punktzahl. Flächen unterscheiden sich
von Feld zu Feld, daher wird alles vor der Bewertung in **Verhältnisse (0–1)** normiert.

```
cov  = min(1, total_worked_m2 / work_area_m2)   wie viel % des Felds tatsächlich bearbeitet
dup  = duplicate_m2 / work_area_m2              wie viel % unnötig zweimal bearbeitet
out  = outside_m2   / work_area_m2              wie viel % Fläche außerhalb der Grenze
ret  = retread_m2   / total_worked_m2           wie viel % der EIGENEN Arbeit noch überfahren ist
outt = Zeit außerhalb der Grenze / elapsed_s    wie viel % der Zeit außerhalb des Felds
```

Jede Kennzahl wird zu einem **Erfüllungsgrad (0–1)**, der **kubiert** und mit der Punktzahl
des Postens multipliziert wird.

```
Postenpunkte = Punkte × Erfüllungsgrad³

SCORE = 45·cov³ + 17·dup_a³ + 16·time_a³ + 7·out_a³ + 6·outt_a³ + 9·ret_a³
```

| Posten | Kennzahl | Bestmarke | Null | Punkte |
|--------|----------|-----------|------|--------|
| Coverage | `cov` | 100% | 0% | **45** |
| Duplicate | `dup` | ≤ 5% | ≥ 25% | **17** |
| Time | `v_eff` | ≥ 3,5 km/h | ≤ 0,5 km/h | **16** |
| Retread | `ret` | ≤ 5% | ≥ 20% | **9** |
| Outside | `out` | 0% | ≥ 10% | **7** |
| Outside Time | `outt` | 0% | ≥ 10% | **6** |

**Die dritte Potenz entscheidet.** Punkte kommen nur nahe der Bestmarke schnell dazu.

| Erfüllungsgrad | 0,5 | 0,7 | 0,8 | 0,9 | 0,95 | 1,0 |
|----------------|-----|-----|-----|-----|------|-----|
| Anteil der Punkte | 13% | 34% | 51% | 73% | 86% | 100% |

Bei 62% Abdeckung bleibt man selbst mit nahezu perfekten übrigen Posten unter 60 Punkten.
**Das Feld bis zum Ende zu füllen zählt mehr als alles andere.**

#### Time sind keine Sekunden, sondern die effektive Arbeitsgeschwindigkeit

Absolute Sekunden hängen von Feldgröße und Arbeitsbreite ab und sind nicht vergleichbar.
Stattdessen wird **die Mindeststrecke zur Abdeckung dieses Felds (`D_ideal`) durch die
tatsächlich benötigte Zeit geteilt**, was eine Geschwindigkeit ergibt.

```
R       = Mindestwendekreis des Traktors (compact 3,2 / medium 4,0 / large 4,9 m)
W       = Arbeitsbreite des Anbaugeräts (m) — aus der Zeile `# implement` der CSV
S       = kurze Seite des minimalen umschließenden Rechtecks des Kartenpolygons (m)
N       = ceil(S / W)                              Anzahl der Überfahrten

D_ideal = work_area_m2 / W + (N − 1) × π × R       (m)
v_eff   = D_ideal / elapsed_s × 3,6                (km/h)
```

`elapsed_s` ist `duration_ms / 1000` und **Sim-Zeit** — im Zeitraffer zu fahren verkürzt das
Ergebnis also nicht.

Bestmarken-Zeiten je Karte für einen großen Traktor (`R = 4,9`) mit großem Pflug (`W = 3,6`):

| Karte | Gewertete Fläche (m²) | `D_ideal` (m) | Bestmarken-Zeit |
|-------|-----------------------|---------------|-----------------|
| M1 Open Fields | 1.499 | 570,5 | **587 s** (9m47s) |
| M2 Sloped Acres | 1.947 | 756,5 | **778 s** (12m58s) |
| M3 Patchwork Plots | 1.356 | 576,6 | **593 s** (9m53s) |

Ein anderer Traktor oder ein anderes Anbaugerät ändert `R` und `W` — dann wird alles neu berechnet.

### 7.3 Ungültige (INVALID) Fahrten

Trifft **einer** der folgenden Punkte zu, wird keine Punktzahl gebildet und die Fahrt aus der
Wertung genommen.

| Bedingung | Erkennung |
|-----------|-----------|
| **Manuelles Fahren** | **Eine oder mehr** Telemetriezeilen mit `manual == 1` |
| **Manuelle Tasteneingabe** | Ein Block `event_t_ms,edge,key` **existiert** |
| **Anbaugerätewechsel** | **Zwei oder mehr** Zeilen `# implement` |
| **Nicht bewertbar** | Keine Zeile `# work_area_m2` oder `# map` |

Fasse während laufender REC-Aufzeichnung die Tastatur nicht an. Einmal ins Lenkrad greifen
oder Gas bzw. Bremse antippen macht die gesamte Fahrt ungültig. Traktor- oder Gerätewechsel
müssen **vor** dem Start der Aufzeichnung abgeschlossen sein.

### 7.4 Abgeschaffte Regeln

Diese Punkte weichen von früheren Angaben ab. **Die folgenden drei gelten nicht mehr.**

| Abgeschaffte Regel | Aktueller Stand |
|--------------------|-----------------|
| **Rückwärtsfahrt setzt gepflügte Fläche zurück** | **Abgeschafft.** Die gemessene Rückwärts-Restfläche liegt nahe null, und die Kosten der Rückwärtsfahrt stecken bereits in Time und Duplicate |
| **Timer läuft außerhalb des Felds 10-fach schneller** | **Entfällt.** Auch aus der App entfernt. Fläche außerhalb der Grenze zählt nun als **Outside**, die Zeit außerhalb als **Outside Time** |
| **Mittlerer Pfadfehler wird am Ziel addiert** | **Kein Bewertungsposten.** Eine Formel wie `finalTimeS = elapsedS + penaltyS` existiert nicht |

Der Pfadfolgefehler wird nicht direkt bewertet, aber ein großer Fehler trägt das Fahrzeug aus
der Spur: `cov` sinkt, `dup` und `ret` steigen — **indirekt** kostet er also viel.

### 7.5 Bewertungsmethode

- Fahre im Emulator mit **eingeschalteter REC-Aufzeichnung** und reiche die gespeicherte Fahraufzeichnung beim Leaderboard ein.
- Eingereicht wird die **versiegelte `.csv.enc`**, die sich nur mit dem Schlüssel der Organisatoren öffnen lässt. Die zusätzlich erzeugte Klartext-CSV dient der eigenen Analyse.
- Lokal gemessene Zeiten und Flächen sind nur Referenz — **offiziell ist der Leaderboard-Wert**.
- Punktzahlen pro Karte werden mit dem **Teamnamen** als Schlüssel summiert.

> Für Punkteverteilung, Toleranzen und den genauen Einreichungsweg ist die Ankündigung der
> Organisatoren maßgeblich. Die Zahlen hier sind aus den Leaderboard-Bewertungsregeln
> übernommen; bei Änderungen gilt die Ankündigung.

### 7.6 Fahrt- / Verbindungs- / Teamnamen-Regeln (strikt einzuhalten)

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
   │   [7] Im Emulator REC einschalten und fahren → verstrichene Zeit / gepflügte Fläche prüfen
   │        (während der Aufzeichnung keine Tastatureingabe — eine genügt für INVALID)
   │        ↓
   │   [8] Gespeicherte Fahraufzeichnung beim Leaderboard einreichen → Aktualisierung prüfen
   │        ↓
   └── [9] Algorithmus / Parameter tunen → zurück zu 4 oder 6

   * Wiederhole die Schleife [6]–[9] bis zur Deadline und verbessere bei jedem Versuch das Leaderboard-Ergebnis.
   * Das beste Ergebnis deines Teams im Leaderboard zum Ablauf der Frist
     (summiert über alle Karten) wird direkt bewertet.
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

**Für jede Karte separat hochladen.** Der Wettbewerb verwendet drei Karten: M1, M2 und M3. Für jede Karte wird die zugehörige RDDF-Datei separat hochgeladen (dateiweise angegeben, z. B. `upload_rddf.sh -f ./1.rddf` — einen mapId-Parameter gibt es nicht). Punktzahlen pro Karte werden mit dem **Teamnamen** als Schlüssel summiert. (§4.5, §7.5)

---

> **F2.** Muss der Flächenanteil 100 % erreichen, um zu gewinnen?

**Es gibt keine Schwelle, die du überschreiten musst.** Coverage hat mit 45 Punkten aber die
größte Gewichtung, und der Erfüllungsgrad wird **kubiert** — eine niedrige Abdeckung lässt die
Punktzahl einbrechen.

| Abdeckung | Erhaltene Punkte (von 45) |
|----------:|--------------------------:|
| 50% | 5,6 |
| 70% | 15,4 |
| 90% | 32,8 |
| 100% | 45,0 |

Etwas mehr Zeit aufzuwenden und das Feld bis zum Ende zu füllen zahlt sich fast immer aus. (§7.2)

---

> **F3.** Gibt es ein separates „finales Einreichungs"-Verfahren?

Du musst für jede Fahrt die **REC-Aufzeichnung (die versiegelte `.csv.enc`) beim Leaderboard
einreichen**, damit sie zählt. Das Einreichen ist die Ergebnisaktualisierung. Was zum Ablauf
der Deadline im Leaderboard steht, wird bewertet — halte also die Schleife aus Hochladen,
Fahren und Einreichen bis dahin am Laufen. (§7.5)

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

**Sie ist nachteilig — und wird direkt abgezogen.** Boden, der die Fertigstellungsschwelle
bereits überschritten hat, erneut zu überfahren zählt als **Duplicate** mit 17 Punkten.

- Bis zu **5% der Feldfläche geben die volle Punktzahl** — etwas Überlappung der Arbeitsbreite ist normale Praxis gegen unbearbeitete Streifen
- Ab **25% gibt es null Punkte**
- Dazwischen wird der Erfüllungsgrad kubiert, sodass schon 10% teuer sind

`Time` leidet ebenfalls — ein doppelter Verlust. (§7.2)

---

> **F8.** Darf ich beim Wenden rückwärts fahren?

**Ja. Die eigene Rückwärts-Penalty wurde abgeschafft.** (§7.4)

Lohnend ist es allerdings selten.

- Jeder Wechsel vorwärts↔rückwärts **hält das Fahrzeug vollständig an** und beschleunigt neu — reiner Zeitverlust → schlechteres `Time`
- Der zurückgefahrene Abschnitt ist bereits bearbeiteter Boden, was `Duplicate` und `Retread` erhöht
- `implementFlag` muss in jedem Rückwärtsabschnitt `0` sein (§4.3)

Ziehe es nur als Alternative zur Dreipunktwende in Betracht, wenn das Feld für einen
Halbkreis-U-turn zu schmal ist.

---

> **F9.** Eine große U-Kurve außerhalb des Felds ist praktisch — wie groß ist die Penalty?

**Einen 10-fach-Timer gibt es nicht.** Stattdessen wird über zwei getrennte Posten abgezogen.

| Posten | Kennzahl | Null bei | Punkte |
|--------|----------|----------|--------|
| Outside | Fläche außerhalb der Grenze ÷ Feldfläche | 10% | 7 |
| Outside Time | Zeit außerhalb ÷ Gesamtzeit | 10% | 6 |

Beide geben **nur bei 0% die volle Punktzahl** — sie gelten als reine Defekte, zu denen dich
nichts zwingt. Die 13 Punkte zusammen sind überschaubar, aber Wenden außerhalb kostet auch
Zeit, sodass `Time` mitleidet. Die Wende innerhalb der Grenze abzuschließen ist die bessere
Wahl. (§7.2)

---

> **F10.** Wie stark wirkt sich der Pfadfolgefehler auf die Punktzahl aus?

**Er ist kein Bewertungsposten.** Auf den Fehler selbst entfallen keine Strafsekunden. (§7.4)

Der indirekte Effekt ist jedoch groß. Trägt es das Fahrzeug aus der Spur, bleiben Zellen
unbearbeitet (`cov` sinkt), es gerät in die Nachbarspur und bearbeitet Bekanntes erneut
(`dup` steigt), und die Räder stehen auf bearbeitetem Boden (`ret` steigt). Auf diese Posten
entfallen zusammen 45 + 17 + 9 = 71 Punkte.

In der App sichtbar sind der Wert **LTD (Querabweichung)** im Dashboard und die Spalte `cte`
in `/tmp/pp_trace.csv`. Wenn sich dieser Wert in einem Kurvenabschnitt kontinuierlich zu
einer Seite aufbaut, hat dieser Abschnitt die Nachführgrenze des Fahrzeugs überschritten.

→ Lässt sich durch höhere **Waypoint-Dichte in Kurvenabschnitten** und **Minimierung von Heading-Sprüngen** reduzieren. (§10.6)

---

### 9.4 Betriebsregeln

> **F11.** Darf ich auf einem Gerät zwei Karten gleichzeitig betreiben?

**Ja — aber die Kombination „gleicher Teamname + gleiche Karte" ist verboten.**

- OK: App A mit M1, App B mit M2 gleichzeitig ausführen
- NG: Zwei Instanzen mit demselben Teamnamen laufen gleichzeitig auf M1
- NG: Mehrere Browser-Tabs, die sich mit derselben App-Instanz verbinden

(§7.6)

---

> **F12.** Darf ich den Teamnamen zwischendurch ändern?

**Nicht empfohlen.** Da der Teamname der Schlüssel für die kartenübergreifende Punktesummierung ist, führt bereits **ein einziges abweichendes Zeichen** dazu, dass das Team als separates Team erkannt und die Punkte getrennt werden. Lege den Teamnamen vor der ersten Fahrt fest und verwende denselben exakten String bei allen Karten und allen Fahrten. (§7.6)

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
├── PathTrackerBase.hpp   Basisklasse, die den Boilerplate-Teil bereits umsetzt
├── SpeedController.hpp   Gang/Gas/Bremse (für alle Algorithmen gemeinsam)
├── SteeringController.hpp Ausgabe des Lenkbefehls
├── TrackerFactory.*      Name → Implementierung
├── TrackerSwitch.*       Tracker-Wechsel zur Laufzeit
├── TrackingLoop.*        Regelschleife (GPS-Umrechnung, Befehle, Telemetrie)
├── SampleClock.hpp       Zeitbasis des Regelzyklus
├── ControlTimeGate.hpp   Gate für den Regelzyklus
├── GpsSampleStore.hpp    Speicher für GPS-Samples
├── SignalFreshness.hpp   Bewertung der Signalaktualität
├── ClockLog.hpp / ClockTelemetry.hpp  Timing-Diagnose
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

Trotz des Namens ist es eine **gewichtete Summe aus fünf Termen**. Die Standardwerte sind so
gesetzt, dass der **Pure-Pursuit-Term führt (Gewicht 0,7) und der Rest nur nachjustiert**.
Pure Pursuit korrigiert Querabweichung und Kursfehler bereits gemeinsam; zieht man die
Hilfsverstärkungen gleich hoch, wird derselbe Fehler dreifach korrigiert, die Lenkung hängt
am Anschlag und es entsteht das S-Pendeln.

| Term | Aufgabe |
|------|---------|
| Krümmungs-Vorsteuerung | Lenkung, die die Krümmung des Pfades selbst verlangt (fehlerunabhängig) |
| Kursfehler | Richtet das Fahrzeug an der Pfadtangente aus |
| Querabweichung | Zieht das Fahrzeug zurück auf den Pfad. Auf Bahnen gemessen an der **um 1,5 s vorausberechneten Position**, in Wenden an der aktuellen Position |
| Pure Pursuit | Kreisbogen zum Vorausschaupunkt — **die Hauptlenkung** (`ppWeightPp`, Standard **0,7**) |
| Gierraten-Dämpfung | Unterdrückt Schwingungen, wenn die Terme oben überreagieren |

### 10.5 ★ Eigenschaften mit direktem Einfluss auf den RDDF-Entwurf

Das ist der praktischste Teil dieses Abschnitts.

| Eigenschaft des Trackers | So schreibst du dein RDDF |
|--------------------------|---------------------------|
| Vorausschaudistanz = **Geschwindigkeit × 9 s**, begrenzt auf `5 m ~ 20 m` (in Kurven kürzer) | Kurven mit engerem Radius werden innen abgeschnitten. Scharfe Wenden entschärfen oder Waypoints dichter setzen |
| Krümmung als gewichteter Mittelwert über **5 aufeinanderfolgende Punkte** um den nächstgelegenen geschätzt | Zu weite Waypoint-Abstände verschlechtern die Schätzung, sodass Bögen untersteuert werden (§4.4 Regel 4) |
| Das Anbaugerät läuft **3 m hinter** dem Fahrzeug | `implementFlag`-Wechsel greifen, wenn das Gerät die Stelle passiert — plane an Bahnanfang und -ende etwa 3 m Reserve ein |
| Verfolgung startet erst **innerhalb von 3 m um Waypoint 0** | Setze den ersten Waypoint auf Punkt A der Karte (§4.4) |
| Ein Wechsel vorwärts↔rückwärts **bremst bis zum Stillstand** | Jeder Richtungswechsel kostet einen kompletten Stopp plus Wiederanfahren, was direkt von `Time` abgeht — vorwärtsbasierte Wenden gewinnen (§7.2) |
| Abschluss = **innerhalb 1 m** um den letzten Waypoint **und** innerhalb der letzten 10 Waypoints | Mache den letzten Waypoint zu einem eindeutigen Endpunkt |
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
