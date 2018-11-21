open Common;

type action =
  | Square(int)
  | GotoHistory(int);

type stateElement = {
  squares: array(option(player)),
  nextMove: player,
  winner: gameStatus,
};

type state = list(stateElement);

let component = ReasonReact.reducerComponent("Game");

let onClick = (self, square, _event) =>
  self.ReasonReact.send(Square(square));

let calculateGameStatus = squares => {
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
  let r: list((option(player), array(int))) =
    List.map(
      a =>
        if (squares[a[0]] != None
            && squares[a[0]] == squares[a[1]]
            && squares[a[0]] == squares[a[2]]) {
          (squares[a[0]], a);
        } else {
          (None, a);
        },
      winningPositions,
    );

  let filtered: list((player, array(int))) =
    Belt.List.keepMap(
      r,
      x => {
        let t = Belt.Option.map(fst(x), y => (y, snd(x)));
        t;
      },
    );

  switch (filtered) {
  | [(PlayerX, position), ..._] => WinnerX(position)
  | [(Player0, position), ..._] => Winner0(position)
  | [] =>
    if (List.for_all(a => a != None, Array.to_list(squares))) {
      Draw;
    } else {
      Continue;
    }
  };
};

let initialState = {
  squares: [|None, None, None, None, None, None, None, None, None|],
  nextMove: PlayerX,
  winner: Continue,
};

let stateOrInitial = state =>
  switch (state) {
  | [hd, ..._] => hd
  | [] => initialState
  };

let handleMove = (state, currentMove) => {
  let currentState = stateOrInitial(state);
  switch (currentState.squares[currentMove], currentState.winner) {
  | (None, Continue) =>
    let (mark, nextMove) =
      switch (currentState.nextMove) {
      | PlayerX => (PlayerX, Player0)
      | Player0 => (Player0, PlayerX)
      };
    let newSquares = Array.copy(currentState.squares);
    newSquares[currentMove] = Some(mark);
    let newState = [
      {
        squares: newSquares,
        nextMove,
        winner: calculateGameStatus(newSquares),
      },
      ...state,
    ];
    ReasonReact.Update(newState);
  | _ => ReasonReact.NoUpdate
  };
};

let handleGotoHistory = (state, historyStep) => {
  let newHistory =
    Belt.Option.getWithDefault(
      Belt.List.drop(state, List.length(state) - historyStep - 1),
      [initialState],
    );
  ReasonReact.Update(newHistory);
};

let make = _children => {
  ...component,
  initialState: () => [initialState],
  reducer: (action, state) =>
    switch (action) {
    | Square(square) => handleMove(state, square)
    | GotoHistory(historyStep) => handleGotoHistory(state, historyStep)
    },
  render: self => {
    let historyClick = (historyStep, _event) =>
      self.ReasonReact.send(GotoHistory(historyStep));
    <div className="game">
      <div className="game-board">
        <Board
          onClick={onClick(self)}
          squares={
            switch (self.state) {
            | [hd, ..._] => hd.squares
            | [] => [|None, None, None, None, None, None, None, None, None|]
            }
          }
        />
      </div>
      <div className="game-info">
        {
          let currentState = stateOrInitial(self.state)
          switch (currentState.winner) {
          | Continue =>
            switch (currentState.nextMove) {
            | PlayerX => ReasonReact.string("Next player X")
            | Player0 => ReasonReact.string("Next player 0")
            }
          | WinnerX(position) => ReasonReact.string("Player X won")
          | Winner0(position) => ReasonReact.string("Player 0 won")
          | Draw => ReasonReact.string("Draw")
          }
        }
        <ol>
          {
            ReasonReact.array(
              Array.of_list(
                List.mapi(
                  (index, element) => {
                    let desc =
                      if (index == 0) {
                        "Go to game start";
                      } else {
                        "Go to step " ++ string_of_int(index);
                      };
                    <li key={string_of_int(index)}>
                      <button onClick={historyClick(index)}>
                        {ReasonReact.string(desc)}
                      </button>
                    </li>;
                  },
                  self.state,
                ),
              ),
            )
          }
        </ol>
      </div>
    </div>;
  },
};