open Common;

let renderSquare = (i, squares, highlight, onClick) =>
  <Square
    value={statusValue(squares[i])}
    onClick={onClick(i)}
    highlight
    key={string_of_int(i)}
  />;

[@react.component]
let make = (~onClick, ~squares, ~winner, ~size) => {
  let squares = squares;
  let highlight =
    switch (winner) {
    | WinnerX(position) => position
    | Winner0(position) => position
    | _ => [||]
    };
  let render = i =>
    renderSquare(
      i,
      squares,
      List.mem(i, Array.to_list(highlight)),
      onClick,
    );

  let renderRow = (i, squares) => {
    let content = squares |> Array.map(render) |> ReasonReact.array;
    <div key={string_of_int(i)} className="board-row"> content </div>;
  };
  <div>
    {Array.init(size, x => x)
     |> Array.map(a => Array.init(size, x => x + a * size))
     |> Array.mapi(renderRow)
     |> ReasonReact.array}
  </div>;
};
