type action =
  | NoAction;

[@react.state]
type state = {value: Common.player};

[@react.component]
let make = (~value, ~onClick, ~highlight) => {
    let cls = "square" ++ (highlight ? " highlight" : "");
    <button className=cls onClick> {ReasonReact.string(value)} </button>;
};