

set(REMOTE_CONTROL_SRCS main.cc Chessboard.cc ChessPiece.cc Pawn.cc Knight.cc Queen.cc Bishop.cc Rook.cc King.cc)
set(REMOTE_CONTROL_HDRS Color.h Chessboard.h ChessPiece.h Pawn.h Knight.h Queen.h Bishop.h Rook.h King.h)

add_executable(app ${REMOTE_CONTROL_SRCS} ${REMOTE_CONTROL_HDRS})

install(TARGETS app RUNTIME DESTINATION bin)
