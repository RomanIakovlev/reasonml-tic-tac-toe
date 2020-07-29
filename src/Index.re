switch (ReactDOM.querySelector("#root")) {
| Some(root) => {
    let url = ReasonReact.Router.dangerouslyGetInitialUrl();

    let size = switch (url.path) {
    | [x] => int_of_string(x)
    | _ => 3
    };
    
    ReactDOM.render(<Game size/>, root)
}
| None => ()
}