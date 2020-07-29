open Common;

type stateElement = {
  squares: array(option(player)),
  nextMove: player,
  winner: gameStatus,
};

type state = list(stateElement);

type action =
  | Square(int)
  | GotoHistory(int);

let onClick = (dispatch, square, _event) => dispatch(Square(square));

[@react.component]
let make = (~size) => {
  let calculateGameStatus = squares => {
    let horizontal =
      List.init(size, x => x)
      |> List.map(a => Array.init(size, x => x + a * size));
    let vertical =
      List.init(size, x => x)
      |> List.map(a => Array.init(size, x => x * size + a));
    let diag1 = Array.init(size, x => x * size + x);
    let diag2 = Array.init(size, x => size - 1 + x * (size - 1));
    let winningPositions = horizontal @ vertical @ [diag1] @ [diag2];

    let winCandidates: list((option(player), array(int))) =
      winningPositions
      |> List.map(winPos => {
           let winningSequenceExists =
             winPos |> Array.for_all(b => squares[b] == squares[winPos[0]]);
           if (squares[winPos[0]] != None && winningSequenceExists) {
             (squares[winPos[0]], winPos);
           } else {
             (None, winPos);
           };
         });

    let filtered: list((player, array(int))) =
      Belt.List.keepMap(
        winCandidates,
        x => {
          let t = Belt.Option.map(fst(x), y => (y, snd(x)));
          t;
        },
      );

    switch (filtered) {
    | [(PlayerX, position), ..._] => WinnerX(position)
    | [(Player0, position), ..._] => Winner0(position)
    | [] =>
      if (Array.to_list(squares) |> List.for_all(a => a != None)) {
        Draw;
      } else {
        Continue;
      }
    };
  };

  let initialState = {
    squares: Array.make(size * size, None),
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
      newState;
    | _ => state
    };
  };

  let handleGotoHistory = (state, historyStep) => {
    let newHistory =
      Belt.Option.getWithDefault(
        Belt.List.drop(state, List.length(state) - historyStep - 1),
        [initialState],
      );
    newHistory;
  };

  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Square(square) => handleMove(state, square)
        | GotoHistory(historyStep) => handleGotoHistory(state, historyStep)
        },
      [initialState],
    );

  let historyClick = (historyStep, _event) =>
    dispatch(GotoHistory(historyStep));

  let currentState = stateOrInitial(state);

  <div className="game">
    <div className="game-info">
      {
         switch (currentState.winner) {
         | Continue =>
           switch (currentState.nextMove) {
           | PlayerX => "Next player X"
           | Player0 => "Next player 0"
           }
         | WinnerX(_) => "Player X won"
         | Winner0(_) => "Player 0 won"
         | Draw => "Draw"
         }
       |> ReasonReact.string}
      <ol>
        {state
         |> List.mapi((index, _element) => {
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
            })
         |> Array.of_list
         |> ReasonReact.array}
      </ol>
    </div>
    <div className="game-board">
      <Board
        onClick={onClick(dispatch)}
        squares={currentState.squares}
        winner={currentState.winner}
        size
      />
    </div>
  </div>;
};
