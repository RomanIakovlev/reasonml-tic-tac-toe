type gameStatus =
  | Continue
  | WinnerX(array(int))
  | Winner0(array(int))
  | Draw;

type player =
  | PlayerX
  | Player0;

let statusValue = buttonStatus =>
  switch (buttonStatus) {
  | None => ""
  | Some(PlayerX) => "X"
  | Some(Player0) => "0"
  };