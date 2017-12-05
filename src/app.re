[%bs.raw {|require('./app.css')|}];

[%bs.raw {|require('photon-ant')|}];

[@bs.module] external logo : string = "./logo.svg";

type action =
  | NewGif(string);

type state = {url: option(string)};

let requestGif = (reduce) =>
  Js.Promise.(
    Fetch.fetch("https://api.giphy.com/v1/gifs/random?api_key=dc6zaTOxFJmzC&tag=cats")
    |> then_(Fetch.Response.json)
    |> then_(
         (json) =>
           {
             Js.log(json); /* Print the resulting json to the console */
             let imageUrlDecoder = Json.Decode.(field("image_url", string));
             let imageUrl = Json.Decode.field("data", imageUrlDecoder, json);
             reduce(() => NewGif(imageUrl), ()); /* Send an action */
             ()
           }
           |> resolve /* Make the then_ chainable */
       )
    |> catch(
         (error) =>
           {
             Js.log(error);
             ()
           }
           |> resolve
       )
    |> ignore /* We're not using the promise itself */
  );

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,
  initialState: () => {url: None}, /* Initial state with no GIF */
  reducer: (action, _state) =>
    /* The reducer */
    switch action {
    /* Pattern matching on the action variant */
    | NewGif(url) =>
      ReasonReact.UpdateWithSideEffects(
        {url: Some(url)},
        ((_self) => Js.log("Received a new GIF: " ++ url))
      )
    },
  didMount: (_self) => ReasonReact.SideEffects((self) => requestGif(self.reduce)),
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
