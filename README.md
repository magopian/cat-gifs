# Cat Gifs

Port of http://elm-lang.org/examples/http in ReasonML and ReasonReact for demo
purposes.

Some documentation:
- ReasonML: https://reasonml.github.io/guide/what-and-why
- ReasonReact: https://reasonml.github.io/reason-react/docs/en/what-why.html
- BuckleScript: https://bucklescript.github.io/docs/en/interop-cheatsheet.html

## Setup

    $ yarn create react-app cat-gifs -- --scripts-version reason-scripts
    $ cd cat-gifs
    $ yarn start

[commit](https://github.com/magopian/cat-gifs/commit/b662c86f61e8f37b51276fe11a62cac4d8d3836b)


## Displaying our first cat GIF

We don't need to display a `message`, so let's get rid of it in `index.re`:

```diff
-ReactDOMRe.renderToElementWithId(<App message="Welcome to React and Reason" />, "root");
+ReactDOMRe.renderToElementWithId(<App />, "root");
```

The compiler helps us along the way, a bit like Flow would (could?):

    Failed to compile.

    ./src/index.re
    Module build failed: Error: We've found a bug for you!
      /Users/mathieuagopian/Dropbox/dev/cat-gifs/src/index.re 5:34-40

      3 │ [@bs.module "./registerServiceWorker"] external register_service_worker
           : unit => unit = "default";
      4 │ 
      5 │ ReactDOMRe.renderToElementWithId(<App />, "root");
      6 │ 
      7 │ register_service_worker();

      This has type:
        (~message: string) =>
        ReasonReact.componentSpec
        (ReasonReact.stateless,  ReasonReact.stateless, 
          ReasonReact.noRetainedProps,  ReasonReact.noRetainedProps, 
          ReasonReact.actionless)
      But somewhere wanted:
        ReasonReact.component ('a,  'b,  'c) (defined as
        at <anonymous>componentSpec ('a,  'a,  'b,  'b,  'c))

This tells us (in a somewhat convoluted way) that the component is expecting a
`message` parameter.

To pass a named parameter to a component, use the following syntax:
`~paramName`.

While it looks like we're writing JSX, we can't just put string content and
expect it to be transformed to an html node. We have to manually call
`ReasonReact.stringToElement`.

[commit](https://github.com/magopian/cat-gifs/commit/e9ef60b5c608d4428a9bd4fb14e921b539c83f65)


## Using Photon and Photon-ant for great good

Using a React component from an external library is doable. Simple even. Unless
it's ant, which requires quite a bit of setup:


```
$ yarn add antd
$ yarn add react-app-rewired --dev
$ yarn add babel-plugin-import --dev
```

Then rewire the scripts in `package.json`:

```diff
-    "start": "react-scripts start",
+    "start": "react-app-rewired start --scripts-version reason-scripts",
-    "build": "react-scripts build",
+    "build": "react-app-rewired build --scripts-version reason-scripts",
-    "test": "react-scripts test --env=jsdom",
+    "test": "react-app-rewired test --env=jsdom --scripts-version reason-scripts",
-    "eject": "react-scripts eject",
+    "eject": "react-app-rewired eject --scripts-version reason-scripts",
```

and add a `config-overrides.json` file:

```
module.exports = function override(config, env) {
  // do stuff with the webpack config...
  return config;
};
```

[commit](https://github.com/magopian/cat-gifs/commit/403d6cfcbdbe885e6c8f980520d9187e7cf2f657)


### Adding Photon

We're not done yet, we'd like to use Photon (and photon-ant) to style our antd
components:

```
$ yarn add photon-ant
$ yarn add react-app-rewire-less --dev
```

Then modify the `config-overrides.js` file:

```diff
 const {injectBabelPlugin} = require('react-app-rewired');
+const rewireLess = require('react-app-rewire-less');

 module.exports = function override(config, env) {
   config = injectBabelPlugin(
-    ['import', {libraryName: 'antd', style: 'css'}],
+    ['import', {libraryName: 'antd', style: true}],
     config,
   );

+  config = rewireLess(config, env);
+
   return config;
 };
```

For antd to use the photon-ant theme, we need to require it, for example in our
`src/app.re` file:

```Reason
[%bs.raw {|require('photon-ant')|}];
```

What that does is to include some raw javascript in our reason file. That's our
first use of javascript interop! It uses reason's adapted syntax of the
bucklescript tool (that compiles reason or ocaml to javascript).

[commit](https://github.com/magopian/cat-gifs/commit/99b2401047756457dd5b56a50d76e0e2bd049c5b)


### And finally using a react (antd) component

You can use any antd component (or react component really) by creating the
appropriate bucklescript binding. Here's an example for the
[Card](https://ant.design/components/card/) component:

```Reason
module Card = {
  [@bs.module "antd"] external card : ReasonReact.reactClass = "Card";
  let make = (~title: option(string)=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=card,
      ~props={"title": Js.Nullable.from_opt(title)},
      children
    );
};
```

Note the `Js.Nullable.from_opt` helper that transforms the `option` 

And then in your reason component:

```Reason
let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: (_self) =>
    <div className="App">
      ...
      <Card title="I'm an antd card title"> (ReasonReact.stringToElement("and I'm an antd card content!")) </Card>
      ...
    </div>
};
```

[comit](https://github.com/magopian/cat-gifs/commit/48b6c5e2950e61ebef17aa2ac6fc2b40b7ce8402)


## Splitting the code in separate files

In the previous step we created a `module` in the `app.re` file. Let's split
that into its own file `Card.re`.

There's no need in Reason to explicitly import modules. If there's a file with
a `.re` extension, it'll be automatically imported and available.

[comit](https://github.com/magopian/cat-gifs/commit/d815ad17d63956d62ee1c78f68d426c9e97ad3e7)


## State and actions FTW

It would be neat to store the GIF url in the state. And update this state using
actions. If this makes you think of [redux](https://redux.js.org/), that's
because it's very similar.

A component that has a state will use a `reducer` to act on those action types,
and update the state or run side effects.

Let's change the `statelessComponent` to a `reducerComponent`:

```Reason
let component = ReasonReact.reducerComponent("App");
```

And define actions using a variant (with only one variant for now):

```Reason
type action =
  | NewGif(string);
```

And finally our state:

```Reason
type state = {url: option(string)};
```

`option` is also a variant (similar to `Option` in Elm). It's either `None` or
`Some("a string")` in our case.

We use a `switch` to unpack a variant (or pattern match):

```Reason
  initialState: () => {url: None}, /* The initial state with no GIF */
  reducer: (action, _state) => /* The reducer */
    switch action { /* Pattern matching on the action variant */
    | NewGif(url) =>
      Js.log("Received a new GIF: " ++ url);
      ReasonReact.NoUpdate
    },
```

Note here that the last expression of a function is the return value. It must
*NOT* end with a `;` or it would be a statement, and not an expression. In that
case, the return value would be `()` (unit).

There's another `switch` in the (new version using the `state` of the) render
method:

```Reason
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
```

[comit](https://github.com/magopian/cat-gifs/commit/f83993543dc2050a3cdf08f17007f2a98efabec7)


## Sending actions (reducing)

In redux you'd use `dispatch` to send an action. In ReasonReact, it's `reduce`.

Let's send the `NewGif` action (and fake it) in the `didMount` lifecycle event:

```Reason
  didMount: (self) => {
    self.reduce(() => NewGif("https://media0.giphy.com/media/3o72EX5QZ9N9d51dqo/giphy.gif"), ());
    ReasonReact.NoUpdate
  },
```

This will fire a `NewGif` action with the "new" GIF url whenever the component
is mounted.

We're not doing anything other than logging a message to the console at the
moment though.

BTW, this was our second use of interop:

```Reason
Js.log("Received a new GIF: " ++ url);
```

[comit](https://github.com/magopian/cat-gifs/commit/9397ecf9bfbf101933932d84b198e8ad3b2aba08)


## Updating the state

It's all well and good to fire actions, but it would be even better if we
reacted to them in a meaningful way (like updating the state).

In the reducer, we can return one of four variant types:

- ReasonReact.NoUpdate: don't do a state update.
- ReasonReact.Update(state): update the state.
- ReasonReact.SideEffects((self) => unit): no state update, but trigger a
  side-effect, e.g. ReasonReact.SideEffects((_self) => Js.log("hello!")).
- ReasonReact.UpdateWithSideEffects(state, (self) => unit): update the state,
  then trigger a side-effect.

This shows us something: what we did previously (triggering a side effect in
the reducer) is bad. Don't do it.

Let's update the code:

```Reason
    | NewGif(url) =>
      ReasonReact.UpdateWithSideEffects(
        {url: Some(url)},
        ((_self) => Js.log("Received a new GIF: " ++ url))
      )
```

This will update the state, and then trigger the side effect.
If we had to update one field of many, we'd do something like:

```Reason
{...state, someField: someValue},
```
