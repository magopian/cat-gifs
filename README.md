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
