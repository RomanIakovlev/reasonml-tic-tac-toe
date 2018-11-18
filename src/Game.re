

let component = ReasonReact.statelessComponent("Game")

let make = (_children) => {
    ...component,
    render: _self => {
        <div className="game">
            <div className="game-board">
                <Board status="Next player X"/>
            </div>
            <div className="game-info">
                <div></div>
                <ol></ol>
            </div>
        </div>
    }
}