# Task Plan: Morse Tree Visualization

## Goal
Implement a real-time visualization in `index.html` that shows how characters are translated to Morse code by traversing a binary tree, based on the logic in `main.cpp`.

## Current Phase
Phase 1: Requirements & Discovery
Phase 2: Design & Planning

## Phases

### Phase 1: Requirements & Discovery
- [x] Understand Morse tree logic in `main.cpp` (Left = Dot, Right = Dash)
- [x] Identify integration point in `index.html`
- [x] Understand user intent
- [x] Identify constraints and requirements
- [x] Document findings in findings.md
- **Status:** complete

### Phase 2: Design & Planning
- [ ] Define the visual structure of the binary tree
- [ ] Create a layout plan for the tree visualization within the existing HUD
- [ ] Map traversal logic to visual state updates (highlighting nodes/paths)
- **Status:** in_progress

### Phase 3: Implementation
- [x] Inject SVG/Canvas container for the tree in `index.html`
- [x] Implement tree construction logic in JS mirroring `main.cpp`
- [x] Create `tracePath(char)` function
- **Status:** complete

### Phase 4: Testing & Refinement
- [ ] Verify each letter correctly traverses the tree
- [ ] Ensure the visualization fits the "Tactical HUD" aesthetic
- [ ] Optimize performance for fast typing
- **Status:** in_progress

### Phase 5: Final Delivery
- [x] Final tree visualization implementation
- [ ] Refine tree size and log placement
- [ ] Add more campus locations
- [ ] Final UI polish and organization
- **Status:** in_progress

## Key Questions
1. Should the entire tree be visible, or only the current branch? (Entire tree preferred for "Tactical" feel)
2. How to handle multiple characters being typed rapidly? (Queue animations or highlight "last" character)

## Decisions Made
| Decision | Rationale |
|----------|-----------|
| Use SVG + Anime.js | Matches existing stack in `index.html` and offers high performance animations |

## Errors Encountered
| Error | Attempt | Resolution |
|-------|---------|------------|
|       | 1       |            |

## Notes
- `main.cpp` uses a binary tree where `dot` is left subtree and `dash` is right subtree.
- Letters are leaf nodes or internal nodes depending on the sequence length.
