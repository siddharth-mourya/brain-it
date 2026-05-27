import { createClient } from '@/lib/supabase/server';
import { Sidebar } from '@/components/sidebar';
import Link from 'next/link';
import { redirect } from 'next/navigation';
import type { Database } from '@/types/database';

type PageSummary = Pick<Database['public']['Tables']['pages']['Row'], 'id' | 'title' | 'updated_at'>;

export default async function Dashboard() {
  const supabase = await createClient();
  const {
    data: { user },
  } = await supabase.auth.getUser();

  if (!user) redirect('/login');

  const { data: pagesData } = await supabase
    .from('pages')
    .select('id,title,updated_at')
    .eq('user_id', user.id)
    .order('updated_at', { ascending: false })
    .limit(20);

  const pages = (pagesData ?? []) as PageSummary[];

  return (
    <main className="flex">
      <Sidebar pages={pages} />
      <section className="w-full p-6">
        <h1 className="text-2xl font-semibold">Dashboard</h1>
        <form
          action={async () => {
            'use server';

            const s = await createClient();
            const {
              data: { user },
            } = await s.auth.getUser();

            if (!user) return;

            const newPage: Database['public']['Tables']['pages']['Insert'] = {
              user_id: user.id,
              title: 'Untitled',
            };

            const { data } = await (s.from('pages') as any).insert(newPage).select('id').single();

            if (data) redirect(`/page/${data.id}`);
          }}
        >
          <button className="mt-4 rounded bg-zinc-900 px-4 py-2 text-white dark:bg-white dark:text-zinc-900">
            Create new page
          </button>
        </form>
        <ul className="mt-6 space-y-2">
          {pages.map((p) => (
            <li key={p.id}>
              <Link className="underline" href={`/page/${p.id}`}>
                {p.title}
              </Link>
            </li>
          ))}
        </ul>
      </section>
    </main>
  );
}
