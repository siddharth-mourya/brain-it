# Brain It

Collaborative internal docs + whiteboard app built with Next.js App Router, Supabase, Tiptap, Excalidraw, TailwindCSS, and Zustand.

## Install

```bash
npm install
npm run dev
```

## Environment

Copy `.env.example` to `.env.local`:

```bash
NEXT_PUBLIC_SUPABASE_URL=...
NEXT_PUBLIC_SUPABASE_ANON_KEY=...
```

## Supabase Setup

1. Create a Supabase project.
2. Run SQL from `supabase/schema.sql` in SQL editor.
3. Verify storage bucket `uploads` exists (public).
4. Enable email/password auth.

## Deploy to Vercel

1. Import repo into Vercel.
2. Set env vars from `.env.example`.
3. Deploy with default Next.js settings.

## Scripts

- `npm run dev`
- `npm run build`
- `npm run start`
- `npm run lint`
- `npm run typecheck`
