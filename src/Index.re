switch (ReactDOM.querySelector("#root")) {
| Some(root) => ReactDOM.render(<Game />, root)
| None => ()
}