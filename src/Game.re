open Common;

type action = {square: int};

type state = {
  squares: list(array(buttonStatus)),
  nextMove: player,
  winner: buttonStatus,
};

let component = ReasonReact.reducerComponent("Game");

let onClick = (self, square, _event) =>
  self.ReasonReact.send({square: square});

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

let make = _children => {
  ...component,
  initialState: () => {
    squares: [[|
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
      Empty,
    |]],
    nextMove: PlayerX,
    winner: Empty,
  },
  reducer: (action, state) =>
    switch (List.hd(state.squares)[action.square], state.winner) {
    | (Empty, Empty) =>
      let (mark, nextMove) =
        switch (state.nextMove) {
        | PlayerX => (MarkedX, Player0)
        | Player0 => (Marked0, PlayerX)
        };
      let newSquares = Array.copy(List.hd(state.squares));
      newSquares[action.square] = mark;
      let currentWinner = calculateWinner(newSquares);
      if (currentWinner == Empty) {
        ReasonReact.Update({
          squares: [newSquares, ...state.squares],
          nextMove,
          winner: currentWinner,
        });
      } else {
        ReasonReact.Update({
          ...state,
          squares: [newSquares, ...state.squares],
          winner: currentWinner,
        });
      };
    | _ => ReasonReact.NoUpdate
    },
  render: self =>
    <div className="game">
      <div className="game-board"> <Board onClick={onClick(self)} squares=List.hd(self.state.squares)/> </div>
      <div className="game-info">
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
        <div />
        <ol />
      </div>
    </div>,
};