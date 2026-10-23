# extension

Standalone sketch for Extension Part 1 and Part 2. It does not use anything from `main_controller`; the pins, the IR calibration and the turn rates are copied in `utils.hpp`.

Set `PART` (and `TASK` for part 1) at the top of `extension.ino`, then upload.

## Files
- `utils.*` holds the sensors and wheels. It is copied from main_controller and adds `readSensors()`, `steer()`, `veerLeft/Right()` and `spinLeft/Right()`.
- `part1.*` covers tasks 1-4. They are built from the essential scenarios and run once from `setup()`.
  - `takeOff()` handles scenarios 5-8, `followUntilWall()` scenario 1, `turnToOpenSide()` scenarios 2/3, `atDeadEnd()` scenario 4.
  - Each task calls the one before it (task2 = task1 + turn + follow, and so on).
- `part2.*` is a millis() state machine using the right-hand wall-following rule, with one step per `loop()`.
  - FOLLOW: steer down the corridor. If our side opens, go to PRE_TURN. If there is a wall ahead, turn our way, otherwise the other way. If the robot is at a dead end, it either finishes or spins out.
  - PRE_TURN: drive `OPENING_MS` more to reach the middle of the gap, then turn.
  - TURNING: a timed spin with no sensor reads, which keeps the timing tight.
  - FINISH: creep up to `FINISH_CM` from the wall, then DONE (stopped, LEDs blinking).
  - Long and short dead ends are told apart by timing how long both sides have been walls. Time × `MS_PER_CM` gives the length of the straight. A length of 1.5 to 2.5 walls counts as long.

## Tune before testing part 2
1. `CELL_CM` in part2.cpp: set to 20 (20 x 20 cm grid).
2. `WALL_CM` in utils.hpp: set to 6, about the side gap when centred. Check it on Serial.
3. `OPENING_MS`: make it bigger if the robot turns into openings too early, smaller if too late.
4. `STEER`: make it bigger if the robot drifts, smaller if it wobbles.
5. Watch Serial for `dead end, straight = XX cm` at each dead end. Short ones should be about 1 wall long, the finish about 2.
