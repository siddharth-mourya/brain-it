"use client";
import Link from "next/link";
import { createClient } from "@/lib/supabase/client";
import { useRouter } from "next/navigation";
export function Sidebar({ pages }: { pages: { id: string; title: string }[] }) {
  const router = useRouter();
  const supabase = createClient();
  return (
    <aside className="w-72 border-r h-screen p-3 hidden md:block">
      <Link href="/dashboard" className="font-semibold text-lg">
        Brain It
      </Link>
      <div className="mt-4 space-y-2">
        {pages.map((p) => (
          <Link
            className="block p-2 rounded hover:bg-zinc-200 dark:hover:bg-zinc-800"
            key={p.id}
            href={`/page/${p.id}`}
          >
            {p.title}
          </Link>
        ))}
      </div>
      <button
        className="mt-4 text-sm underline"
        onClick={async () => {
          await supabase.auth.signOut();
          router.push("/login");
          router.refresh();
        }}
      >
        Logout
      </button>
    </aside>
  );
}
