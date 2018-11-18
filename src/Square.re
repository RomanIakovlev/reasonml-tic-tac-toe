type action = 
    | NoAction

type state = {
    value: Common.buttonStatus
};

let component = ReasonReact.statelessComponent("Square");

let make = (~value, ~onClick, _children) => {
    ...component,
    render: _self => {
        <button className="square" onClick>
            {ReasonReact.string(value)}
        </button>
        },
};