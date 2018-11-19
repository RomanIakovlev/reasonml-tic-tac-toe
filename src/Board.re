open Common;

let component = ReasonReact.statelessComponent("Board");

let renderSquare = (i, squares, onClick) =>
  <Square
    value={statusValue(squares[i])}
    onClick={onClick(i)}
  />;

let make = (~onClick, ~squares, _children) => {
  ...component,
  render: _self =>
    <div>
      <div className="board-row">
        {renderSquare(0, squares, onClick)}
        {renderSquare(1, squares, onClick)}
        {renderSquare(2, squares, onClick)}
      </div>
      <div className="board-row">
        {renderSquare(3, squares, onClick)}
        {renderSquare(4, squares, onClick)}
        {renderSquare(5, squares, onClick)}
      </div>
      <div className="board-row">
        {renderSquare(6, squares, onClick)}
        {renderSquare(7, squares, onClick)}
        {renderSquare(8, squares, onClick)}
      </div>
    </div>,
};