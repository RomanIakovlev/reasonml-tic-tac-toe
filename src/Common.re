type buttonStatus =
    | Empty
    | MarkedX
    | Marked0

let statusValue = (buttonStatus) => switch(buttonStatus) {
    | Empty => ""
    | MarkedX => "X"
    | Marked0 => "0"
}

type player =
    | PlayerX
    | Player0