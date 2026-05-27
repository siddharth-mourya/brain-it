import { createClient } from "@/lib/supabase/server";
import { Sidebar } from "@/components/sidebar";
import Link from "next/link";
import { redirect } from "next/navigation";
import type { Database } from "@/types/database";

type PageSummary = Pick<
  Database["public"]["Tables"]["pages"]["Row"],
  "id" | "title" | "updated_at"
>;

async function createPageAction() {
  "use server";

  const supabase = await createClient();
  const {
    data: { user },
  } = await supabase.auth.getUser();

  if (!user) redirect("/login");

  const newPage: Database["public"]["Tables"]["pages"]["Insert"] = {
    user_id: user.id,
    title: "Untitled",
  };

  const { data } = await (supabase.from("pages") as any)
    .insert(newPage)
    .select("id")
    .single();

  if (data) redirect(`/page/${data.id}`);
}

export default async function Dashboard() {
  const supabase = await createClient();
  const {
    data: { user },
  } = await supabase.auth.getUser();

  if (!user) redirect("/login");

  const { data: pagesData } = await supabase
    .from("pages")
    .select("id,title,updated_at")
    .eq("user_id", user.id)
    .order("updated_at", { ascending: false })
    .limit(20);

  const pages = (pagesData ?? []) as PageSummary[];

  return (
    <main className="min-h-screen bg-gradient-to-br from-zinc-50 via-white to-zinc-100 text-zinc-900 dark:from-zinc-950 dark:via-zinc-900 dark:to-zinc-950 dark:text-zinc-100">
      <div className="flex min-h-screen">
        <Sidebar pages={pages} />
        <section className="w-full p-6 md:p-10">
          <div className="mx-auto max-w-4xl space-y-8">
            <header className="rounded-2xl border border-zinc-200 bg-white/80 p-6 shadow-sm backdrop-blur dark:border-zinc-800 dark:bg-zinc-900/80">
              <div className="flex flex-wrap items-center justify-between gap-4">
                <div>
                  <h1 className="text-3xl font-bold tracking-tight">Dashboard</h1>
                  <p className="mt-1 text-sm text-zinc-600 dark:text-zinc-400">
                    Manage your pages and continue writing where you left off.
                  </p>
                </div>
                <form action={createPageAction}>
                  <button className="rounded-xl bg-zinc-900 px-5 py-2.5 text-sm font-medium text-white transition hover:bg-zinc-700 dark:bg-white dark:text-zinc-900 dark:hover:bg-zinc-200">
                    Create new page
                  </button>
                </form>
              </div>
            </header>

            <div className="rounded-2xl border border-zinc-200 bg-white/90 p-4 shadow-sm dark:border-zinc-800 dark:bg-zinc-900/80">
              <h2 className="px-2 text-sm font-semibold uppercase tracking-wide text-zinc-500 dark:text-zinc-400">
                Recent pages
              </h2>
              {pages.length === 0 ? (
                <p className="px-2 py-8 text-sm text-zinc-500 dark:text-zinc-400">
                  You don&apos;t have any pages yet. Create your first page to get
                  started.
                </p>
              ) : (
                <ul className="mt-3 divide-y divide-zinc-200 dark:divide-zinc-800">
                  {pages.map((p) => (
                    <li key={p.id}>
                      <Link
                        className="flex items-center justify-between rounded-lg px-3 py-3 transition hover:bg-zinc-100 dark:hover:bg-zinc-800"
                        href={`/page/${p.id}`}
                      >
                        <span className="font-medium text-zinc-800 dark:text-zinc-100">
                          {p.title}
                        </span>
                        <span className="text-xs text-zinc-500 dark:text-zinc-400">
                          {new Date(p.updated_at).toLocaleString()}
                        </span>
                      </Link>
                    </li>
                  ))}
                </ul>
              )}
            </div>
          </div>
        </section>
      </div>
    </main>
  );
}
