1. Aufgabe aus Computational Geometry
=====================================

Jakob Schöttl, Markus Stampfl


Implmentierung
--------------

Im ersten Schritt haben wir die Aufgabe in C++ gelöst.  Das Programm findet
sich im Ordner `cpp/` und kann über das darin enthaltenen Makefile erstellt
werden.

Für den Algorithmus haben wir auf Papier alle möglichen Fälle aufgezeichnet,
und konnten durch jedes weitere if-Statement in der Funktion `intersect` eine
weitere Gruppe abhaken, bis alle Fälle behandelt waren.

Wir haben hier nach dem Ansatz "return early" programmiert.  Dadurch ist der
Code leicht zu lesen und nachzuvollziehen, weil mit jedem return-Statement
ein weiterer Fall einfach abgeschlossen ist, und man diesen nicht mehr im
Hinterkopf behalten muss.  Außerdem spart man sich Variablen und Einrückungen,
was auch zu sauberem Code beiträgt.

Das C++-Programm haben wir dann in die funktionale Programmiersprache Haskell
übersetzt.  Diese Sprache lerne ich gerade in einer anderen Vorlesung.  Der
Algorithmus selbst ist in Haskell noch prägnanter; das Drumherum war eine
größere Herausforderung.  Die Haskell-Implementierung liegt in `haskell/Main.hs`.
Als Build-System verwenden wir Cabal.  Mit `cabal build` in `haskell` sollte
das Programm erzeugt werden.


Aufruf der Programme
--------------------

Beide Programme lesen die Eingabedaten von der Standardeingabe.  Die Laufzeit
kann unter Linux mit dem Programm `time` ermittelt werden.  Beispiel:

    time cpp/intersect < data/test.dat

Um den Programmaufruf mit den verschiedenen ausführbaren Dateien und
Eingabedateien zu vereinfachen gibt es das Skript `test/run-intersect.sh`.


Test der Programme
------------------

Anhand einer kleinen Beispieldatei haben wir unser Programm getestet:
`data/test.dat`.

![Beispieldatei]()

Dazu wird das Skript `test/run-test.sh` verwendet, das unser Programm
mit folgenden Eingaben testet:

* Liste der Strecken in richtiger und in auch in umgekehrter Reihenfolge.
* Dazu jeweils Anfang- und Endpunkte der Strecken in richtiger Reihenfolge
  aber auch vertauscht.

Test erfolgreich: Es kommen immer acht Schnittpunkte heraus.


Laufzeituntersuchungen
----------------------

Die C++-Implementierung ist wie erwartet schneller als das Haskell-Programm.
Mit Haskell arbeitet man einfach auf einer höheren Abstraktionsebene, mit
Listen und map...  Und so gut kann der Compiler diese Unterschiede wohl nicht
wegoptimieren.  Die Daten hierzu liegen in `test-c-vs-h/` und können mittels
dem Makefile reproduziert werden.

Die Laufzeit von Haskell hat uns anfangs Schwierigkeiten bereitet, da sie
nicht quadratische Laufzeit sondern O(n^4) hatte.  Und das, obwohl ich nur
*nacheinander* (nicht verschachtelt) zwei Funktionen quadratischer Laufzeit
verwendet habe.  Grund war, dass die erste Funktion eine Liste mit n^2
erzeugt hat, und auf diese neue, viel längere Liste wurde die zweite Funktion
losgelassen.  Damit ist die Laufzeit dann natürlich O((n^2)^2) = O(n^4).
Jetzt ist es natürlich schlauer implementiert.  Vgl. ....
Die Ergebnisse liegen in `test-haskell-with-nubBy/` und `test-haskell/`.


Ergebnisse
----------

Beide unsere Implementierungen kommen auf die gleichen Ergebnisse:


Maschinenengenauigkeit und Epsilon
----------------------------------


Parallelisierbarkeit
--------------------

Wir haben uns auch noch Gedanken gemacht, wie die Rechenzeit verkürzt
werden könnte, mit möglichst geringen Eingriffen in das Programm.  Eine gute
und einfache Möglichkeit wäre, dem Programm einen Kommandozeilenparameter
mitzugeben, der sagt bis zu welcher Zeile in der Eingabedatei die äußere
Schleife laufen soll.  Die äußere Schleife, ist die, die nacheinander eine
Strecke herausgreift und gegen alle folgenden Strecken auf Schnittpunkte testet.

Angenommen, die Eingabedatei `test.dat` hat 1000 Zeilen. Dann kann das
Programm auf einem Zweikernprozessor wie folgt parallel gestartet werden:

1. ./intersect -l 300 < test.dat
2. awk 'NR>300' test.dat | ./intersect

Dieser Ansatz kann für beliebig viele Prozessoren erweitert werden.
Die Zahlen sind aber hier rein geschätzt, lassen sich aber natürlich
auch sinnvoll berechnen.
