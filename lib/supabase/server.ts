import { createServerClient, type CookieOptions } from "@supabase/ssr";
import { cookies } from "next/headers";
import type { Database } from "@/types/database";

const supabaseUrl = process.env.NEXT_PUBLIC_SUPABASE_URL;
const supabaseKey = process.env.NEXT_PUBLIC_SUPABASE_PUBLISHABLE_KEY;

export const createClient = async () => {
  const cookieStore = await cookies();

  return createServerClient<Database>(supabaseUrl!, supabaseKey!, {
    cookies: {
      getAll: () => cookieStore.getAll(),
      setAll: (
        cookiesToSet: {
          name: string;
          value: string;
          options: CookieOptions;
        }[],
      ) => {
        cookiesToSet.forEach(({ name, value, options }) =>
          cookieStore.set(name, value, options),
        );
      },
    },
  });
};
