# Test Cases and Results

## Project
- File under test: `main.cpp`
- Executable: `morse_sim` (`morse_sim.exe` on Windows)
- Test date: 2026-04-16
- Environment: Windows PowerShell

## Build Step
- Command used:
```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o morse_sim main.cpp
```

## Test Summary
- Total test cases: 6
- Passed: 5
- Failed: 1
- Notes: 1 failed case reveals an edge-case bug when source/destination node does not exist in the graph.

## Detailed Test Cases

### TC-01: View campus nodes from menu
- Objective: Verify that default nodes are present and menu option 3 works.
- Input sequence:
```text
3
4
```
- Expected result:
  - Program prints available nodes.
  - Includes `Admin`, `Hostel`, `Lab`, `Library`.
- Actual result:
  - Output displayed: `Available Nodes: [Admin] [Hostel] [Lab] [Library]`
- Status: PASS

### TC-02: Send message on default shortest path
- Objective: Verify Morse encode/decode and Dijkstra route for known nodes.
- Input sequence:
```text
1
Hostel
Admin
SOS
4
```
- Expected result:
  - Shortest path: `Hostel -> Lab -> Admin`
  - Total cost: `6`
  - Morse signal for SOS shown as `... --- ...`
  - Final decoded message: `SOS`
- Actual result:
  - Path: `Hostel -> Lab -> Admin`
  - Cost: `6 units`
  - Morse: `... --- ...`
  - Final decoded message: `SOS`
- Status: PASS

### TC-03: Unknown destination node handling
- Objective: Verify behavior when destination does not exist.
- Input sequence:
```text
1
Hostel
Canteen
HELLO
4
```
- Expected result:
  - Transmission should fail with no route found.
- Actual result:
  - Program starts transmission and shows:
    - Shortest path: `Canteen`
    - Total cost: `0 units`
    - Final decoded message: `HELLO`
- Status: FAIL
- Defect observed:
  - Missing node validation in `findShortestPath` allows an invalid path/cost to be produced when destination is absent.

### TC-04: Add new connection then send message
- Objective: Verify menu option 2 updates graph and impacts routing.
- Input sequence:
```text
2
Lab
Canteen
1
1
Hostel
Canteen
HELLO
4
```
- Expected result:
  - Connection added successfully.
  - Shortest path should include new edge: `Hostel -> Lab -> Canteen`.
  - Total cost should be `3` (2 + 1).
  - Final decoded message should be `HELLO`.
- Actual result:
  - `Network updated successfully.`
  - Path: `Hostel -> Lab -> Canteen`
  - Cost: `3 units`
  - Final decoded message: `HELLO`
- Status: PASS

### TC-05: Unsupported punctuation in message
- Objective: Verify handling of characters not present in Morse map (e.g., `!`).
- Input sequence:
```text
1
Hostel
Admin
HI!
4
```
- Expected result:
  - Unsupported punctuation is skipped during encoding.
  - Final decoded message should be `HI`.
- Actual result:
  - Morse shown as `.... ..`
  - Final decoded message: `HI`
- Status: PASS

### TC-06: Lowercase and word spacing
- Objective: Verify lowercase normalization and space handling.
- Input sequence:
```text
1
Hostel
Admin
hi there
4
```
- Expected result:
  - Text is encoded from lowercase by converting to uppercase.
  - Word gap is preserved in Morse spacing.
  - Final decoded output should be uppercase: `HI THERE`.
- Actual result:
  - Morse: `.... ..    - .... . .-. .`
  - Final decoded message: `HI THERE`
- Status: PASS

## Known Issue Found During Testing
- Issue: Unknown node handling in shortest path.
- Impact: Can produce incorrect success output (`path = destination only`, `cost = 0`) when `start` or `end` node is not part of the graph.
- Recommended fix:
  - Validate `start` and `end` exist in `nodes` before running Dijkstra.
  - If either is absent, return empty path and keep failure flow in `main()`.

## Suggested Additional Tests (Not Yet Executed)
- Invalid menu input (`abc` instead of integer).
- Negative edge weight input (Dijkstra should reject or sanitize).
- Very long message performance test.
- Multiple equal-cost shortest paths consistency test.
