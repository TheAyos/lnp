# Logic and Proofs

Project made by Youssef Seghrouchni, Siyuan Lu, Ayoub Agouzoul and Rachid Tazi.

## Initial Setup
The first step is to create a local copy of the repository on your computer.

1. Open a terminal and navigate to the folder where you want to store the project.
2. Run the following command:
   ```bash
   git clone git@gitlab.binets.fr:rachid.tazi/logic-and-proofs.git
   ```
## Coding Workflow : **Each time** you want to code !
1. ```git fetch```
2. Ensure you are on your own development branch ```git checkout YOURNAME``` (you can see the branch names with ```git branch```)
3. ```git pull --rebase```
4. Code...
5. Stage your changed files : ```git add .```
6. Commit your updates : ```git commit -am "[filename] fix X"```
7. ```git push -u origin YOURNAME```
8. Done ! Your changes are synced with the server.

## Building the code
1. See teacher's example on github to modify CMakeLists.txt
2. Use ```build.sh``` (unix only)
3. The built binaries should be in ```build/``` (e.g. run with ```./build/mainProgram```)


# Next steps : 
(Template : - [ ] TASK --> assignee)
- [x] MAKEFILE to do
- [ ] USE the library 'fstream' to parse the input file and generate an output file --> ayoub
- [ ] start implementing the pieces and rules
- [ ] start searching algorithms for move predictions

3. Modelize the board and pieces
- from a1 to h8 : ideas of functions
- [ ] is_valid(pos) : in the board ?
- [ ] check_pos(pos) : if a pawn is at pos, return the type of the pawn

- pawn, knight, rook, bishop, queen, king

4. Modelize basic moves
each move is described as follow : (srcLetter)(srcNumber)(destLetter)(destNumber){(promotion code)}

5. CHECK detection
- we can do it by propagation : we start at the king position and increase the size of the square each time to see if the line or the diagonal is protected or not reached by the ennemy. 
- and check all knight positions (max 8)