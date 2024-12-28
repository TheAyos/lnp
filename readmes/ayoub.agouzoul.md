### Week 1:
- Setup explanatory steps in README for efficient git collaboration
- Setup separate development branches to avoid conflicts and to protect main branch
- Setup intial files (gitignore, CMakeLists...)
### Week 2:
- Fixed a bunch of merge conflicts
- Finished file IO (arg parsing, history file parsing, and move output to correct file)
### Week 3:
- Improved performance of the AI by using alpha-beta pruning instead of minimax
- Suggested using a C++ profiler to target optimization efforts (gprof, std::vector functions use 15% of cpu time, switching to bitboard representation will improve performance)
### Week 4:
- Moved the project to bitboard representation, optimizing movegen speed by around 5-10x
- Added FEN parsing to test tricky positions
- Optimized speed of bit operations further
- Fixed movegen bugs that appeared at depth 7
- Added a PerftTests testing suite