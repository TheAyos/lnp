#!/bin/bash
# small bash script to run multiple games and count the results :)

# command="chessGame -W ./build/logic-and-proofs -B chessDeterministicPlayer"
#command="chessGame -W ./build/logic-and-proofs -B chessRandomPlayer"
command="chessGame -W ./build/logic-and-proofs -B chessRandomPlayer"

if [ -z "$1" ]; then
    echo "Usage: $0 <number_of_games>"
    exit 1
fi

games_to_play=$1

white_wins=0
black_wins=0
draws=0
errors=0

echo "Executing command '$command' for $games_to_play games..."

for ((game = 1; game <= games_to_play; game++)); do
    echo "Running game $game..."

    output=$($command 2>&1 | tr -d '\000') # fix ./games.sh: line 18: warning: command substitution: ignored null byte in input ??? whatever this is
    exit_status=$?

    if [[ $exit_status -ne 0 ]]; then
        echo "Error in game $game: "
        ((errors++))
        # continue
    fi

    fulloutput=$output
    output=$(echo "$output" | tail -n 1)
    echo "$output"

    if [[ $output == *"Draw! Game over."* ]]; then
        ((draws++))
    elif [[ $output == *"The White player wins!"* ]]; then
        ((white_wins++))
    elif [[ $output == *"The Black player wins!"* ]]; then
        ((black_wins++))
    else
        echo "Unexpected output in game $game: "
	echo $fulloutput > "$game.log"
    fi

    sleep 0.01
done

echo "Results after $games_to_play games:"
echo "White wins: $white_wins"
echo "Black wins: $black_wins"
echo "Draws: $draws"
echo "Errors: $errors"

# command="chessGame -W ./build/logic-and-proofs -B chessRandomPlayer"
# Results after 100 games:
# White wins: 31
# Black wins: 11
# Draws: 58
# Errors: 0
