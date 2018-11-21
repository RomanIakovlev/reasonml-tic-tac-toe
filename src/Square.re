type action =
  | NoAction;

type state = {value: Common.player};

let component = ReasonReact.statelessComponent("Square");

let make = (~value, ~onClick, ~highlight, _children) => {
  ...component,
  render: _self => {
    let cls = "square" ++ (highlight ? " highlight" : "");
    <button className=cls onClick> {ReasonReact.string(value)} </button>;
  },
};