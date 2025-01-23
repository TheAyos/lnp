import pandas as pd
import chess
import re

file_path = 'chess_openings.csv'  
output_file_path = 'openings_transformed8.txt'  

#read_csv
chess_openings_df = pd.read_csv(file_path)

#check if there is move column
if 'moves' not in chess_openings_df.columns:
    raise ValueError("no moves column")
chess_openings_df['moves'] = chess_openings_df['moves'].fillna('')  #replace NaN

starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

# dictionary for fen
fen_to_moves = {}

#clean move
def clean_moves(moves):
    return re.sub(r'\b\d+\.\s*', '', moves)


with open(output_file_path, 'w') as output_file:
    for index, row in chess_openings_df.iterrows():
        moves = clean_moves(row['moves'])
        board = chess.Board(starting_fen)  #initialize board
        
        try:
            for i in range(len(moves.split())) : 
                move = moves.split()[i]
                san_move = move.strip()
                fen = board.fen()  

                #into UCI
                uci_move = board.parse_san(san_move).uci()

                if fen not in fen_to_moves:
                    fen_to_moves[fen] = set() 
                fen_to_moves[fen].add(uci_move)

                board.push_uci(uci_move)

        except Exception as e:
            print(f"error for the line {index}: {e}")

    # write 
    for fen, moves in fen_to_moves.items():
        moves_str = " ".join(sorted(moves))  
        output_file.write(f"{fen} {moves_str}\n")


