[%bs.raw {|require('./app.css')|}];

[%bs.raw {|require('photon-ant')|}];

[@bs.module] external logo : string = "./logo.svg";

/* Bucklescript binding to an external react component. */
module Card = {
  /* Use the `antd.Card` component */
  [@bs.module "antd"] external card : ReasonReact.reactClass = "Card";
  /* Create a `make` function that wraps the react component so it's usable by */
  /* ReasonReact's JSX. */
  let make = (~title: option(string)=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=card,
      ~props={"title": Js.Nullable.from_opt(title)},
      children
    );
};

let component = ReasonReact.statelessComponent("App");

let make = (_children) => {
  ...component,
  render: (_self) =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement("Cats")) </h2>
      </div>
      <p>
        <Card title="Some cat GIF">
          <img
            src="https://media0.giphy.com/media/3o72EX5QZ9N9d51dqo/giphy.gif"
            alt="some cat gif"
          />
        </Card>
      </p>
    </div>
};
