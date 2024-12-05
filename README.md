# Logic and Proofs

Project made by Youssef Seghrouchni, Siyuan Lu, Ayoub Agouzoul and Rachid Tazi.

## Initial Setup
The first step is to create a local copy of the repository on your computer.

1. Open a terminal and navigate to the folder where you want to store the project.
2. Run the following command:
   ```bash
   git clone git@gitlab.binets.fr:rachid.tazi/logic-and-proofs.git
   ```

## Building the code
1. See teacher's example on github to modify CMakeLists.txt
2. Make sure that the script is executable ```chmod +x build.sh```
3. Use ```build.sh``` (unix only)
4. The built binaries should be in ```build/``` (e.g. run with ```./build/logic-and-proofs```)

## Running the code
- ```./build/logic-and-proofs -H history_ex_1.txt -m move.txt```
- => should output the parsed moves in the terminal and write 'g1h3' into ```move.txt```

## Coding Workflow : **Each time** you want to code !
~~1. ```git fetch```
2. Ensure you are on your own development branch ```git checkout YOURNAME``` (you can see the branch names with ```git branch```)
3. When starting to work on a new feature, ensure your branch has the latest updates from the development branch : ```git rebase dev && git push```
   (Otherwise, just to make sure your local and remote branch are synced : ```git pull --rebase```)
4. Code.....
5. Stage your changed files : ```git add .```
6. Commit your updates : ```git commit -am "[filename] fix X"```
7. ```git push -u origin YOURNAME```
8. Done ! Your changes are synced with the server.~~

For now, we'll just work on the dev branch :
- Make sure you are on the dev branch : ```git checkout dev```
- Make sure to commit all your updates ```git add ...``` and ```git commit -am "message..."```
- Sync the changes that others did with your local changes ```git pull --rebase``` and sync back to the remote repository ```git push``` 


## Merging your changes to the development branch
~~1. Completing the "Coding Workflow" steps above
2. Go to gitlab, on the project's page and 'create merge request'
3. Assign the merge request to someone other than you and add a reviewer
4. UNCHECK 'delete source branch when merge request is accepted'
5. Create merge request~~

## NB
- output file should be one line only (next move)
- for castling, rook move comes always first

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