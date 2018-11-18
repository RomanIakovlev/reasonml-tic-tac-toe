open Common;

type action = {square: int};

type state = {
  squares: array(buttonStatus),
  nextMove: player,
  winner: buttonStatus,
};

let component = ReasonReact.reducerComponent("Board");

let onClick = (square, self, _event) =>
  self.ReasonReact.send({square: square});

let renderSquare = (i, self) =>
  <Square
    value={statusValue(self.ReasonReact.state.squares[i])}
    onClick={onClick(i, self)}
  />;

let calculateWinner = squares => {
  let winningPositions = [
    [|0, 1, 2|],
    [|3, 4, 5|],
    [|6, 7, 8|],
    [|0, 3, 6|],
    [|1, 4, 7|],
    [|2, 5, 8|],
    [|0, 4, 8|],
    [|2, 4, 6|],
  ];
  let r =
    List.map(
      a =>
        if (squares[a[0]] != Empty
            && squares[a[0]] == squares[a[1]]
            && squares[a[0]] == squares[a[2]]) {
          squares[a[0]];
        } else {
          Empty;
        },
      winningPositions,
    );
  try (List.find(a => a != Empty, r)) {
  | Not_found => Empty
  };
};

let make = (~status, _children) => {
  ...component,
  initialState: () => {
    squares: [|
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
    |],
    nextMove: PlayerX,
    winner: Empty,
  },
  reducer: (action, state) =>
    switch (state.squares[action.square], state.winner) {
    | (Empty, Empty) =>
      let (mark, nextMove) =
        switch (state.nextMove) {
        | PlayerX => (MarkedX, Player0)
        | Player0 => (Marked0, PlayerX)
        };
      let newSquares = Array.copy(state.squares);
      newSquares[action.square] = mark;
      let currentWinner = calculateWinner(newSquares);
      if (currentWinner == Empty) {
        ReasonReact.Update({
          squares: newSquares,
          nextMove,
          winner: currentWinner,
        });
      } else {
        ReasonReact.Update({
          ...state,
          squares: newSquares,
          winner: currentWinner,
        });
      };
    | _ => ReasonReact.NoUpdate
    },
  render: self =>
    <div>
      <div className="status">
        {
          switch (self.state.winner) {
          | Empty =>
            switch (self.state.nextMove) {
            | PlayerX => ReasonReact.string("Next player X")
            | Player0 => ReasonReact.string("Next player 0")
            }
          | MarkedX => ReasonReact.string("Player X won")
          | Marked0 => ReasonReact.string("Player 0 won")
          }
        }
      </div>
      <div className="board-row">
        {renderSquare(0, self)}
        {renderSquare(1, self)}
        {renderSquare(2, self)}
      </div>
      <div className="board-row">
        {renderSquare(3, self)}
        {renderSquare(4, self)}
        {renderSquare(5, self)}
      </div>
      <div className="board-row">
        {renderSquare(6, self)}
        {renderSquare(7, self)}
        {renderSquare(8, self)}
      </div>
    </div>,
};