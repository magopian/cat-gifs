/* Bucklescript binding to an external react component. */
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
