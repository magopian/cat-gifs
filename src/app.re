[%bs.raw {|require('./app.css')|}];

[%bs.raw {|require('photon-ant')|}];

[@bs.module] external logo : string = "./logo.svg";

type action =
  | NewGif(string);

type state = {url: option(string)};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,
  initialState: () => {url: None}, /* Initial state with no GIF */
  reducer: (action, _state) =>
    /* The reducer */
    switch action {
    /* Pattern matching on the action variant */
    | NewGif(url) =>
      Js.log("Received a new GIF: " ++ url);
      ReasonReact.NoUpdate
    },
  didMount: (self) => {
    self.reduce(() => NewGif("https://media0.giphy.com/media/3o72EX5QZ9N9d51dqo/giphy.gif"), ());
    ReasonReact.NoUpdate
  },
  render: ({state}) =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement("Cats")) </h2>
      </div>
      <p>
        <Card title="Some cat GIF">
          (
            switch state.url {
            | Some(url) => <img src=url alt="some cat gif" />
            | None => <Spin />
            }
          )
        </Card>
      </p>
    </div>
};
