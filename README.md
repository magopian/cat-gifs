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
