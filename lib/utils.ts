export const cn = (...s: Array<string | false | null | undefined>) =>
  s.filter(Boolean).join(" ");
