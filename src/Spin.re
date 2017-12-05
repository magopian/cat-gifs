/* Bucklescript binding to an external react component. */
/* Use the `antd.Spin` component */
[@bs.module "antd"] external spin : ReasonReact.reactClass = "Spin";

/* Create a `make` function that wraps the react component so it's usable by */
/* ReasonReact's JSX. */
let make = (children) =>
  ReasonReact.wrapJsForReason(~reactClass=spin, ~props=Js.Obj.empty(), children);
