/* Bucklescript binding to an external react component. */
/* Use the `antd.Button` component */
[@bs.module "antd"] external button : ReasonReact.reactClass = "Button";

type buttonType =
  | Default
  | Primary
  | Ghost
  | Dashed
  | Danger;

/* Create a `make` function that wraps the react component so it's usable by */
/* ReasonReact's JSX. */
let make =
    (
      ~icon: option(string)=?,
      ~type_: buttonType=Default,
      ~onClick: option((unit => unit))=?,
      children
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=button,
    ~props={
      "icon": Js.Nullable.from_opt(icon),
      "type":
        switch type_ {
        | Default => "default"
        | Primary => "primary"
        | Ghost => "ghost"
        | Dashed => "dashed"
        | Danger => "Danger"
        },
      "onClick": Js.Nullable.from_opt(onClick)
    },
    children
  );
