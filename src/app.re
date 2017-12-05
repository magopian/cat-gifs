[%bs.raw {|require('./app.css')|}];

[%bs.raw {|require('photon-ant')|}];

[@bs.module] external logo : string = "./logo.svg";

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
        <img
          src="https://media0.giphy.com/media/3o72EX5QZ9N9d51dqo/giphy.gif"
          alt="some cat gif"
        />
      </p>
    </div>
};
