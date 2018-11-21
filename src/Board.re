open Common;

let component = ReasonReact.statelessComponent("Board");

let renderSquare = (i, squares, highlight, onClick) =>
  <Square value={statusValue(squares[i])} onClick={onClick(i)} highlight />;

let make = (~onClick, ~squares, ~winner, _children) => {
  ...component,
  render: _self => {
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
    <div>
      <div className="board-row"> {render(0)} {render(1)} {render(2)} </div>
      <div className="board-row"> {render(3)} {render(4)} {render(5)} </div>
      <div className="board-row"> {render(6)} {render(7)} {render(8)} </div>
    </div>;
  },
};