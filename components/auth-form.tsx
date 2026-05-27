"use client";
import { useState } from "react";
import { createClient } from "@/lib/supabase/client";
import { useRouter } from "next/navigation";
import { toast } from "sonner";
export function AuthForm({ mode }: { mode: "login" | "signup" }) {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [loading, setLoading] = useState(false);
  const router = useRouter();
  const submit = async (e: React.FormEvent) => {
    e.preventDefault();
    setLoading(true);
    const supabase = createClient();
    const fn =
      mode === "login"
        ? supabase.auth.signInWithPassword
        : supabase.auth.signUp;
    const { error } = await fn({ email, password });
    setLoading(false);
    if (error) return toast.error(error.message);
    toast.success(mode === "login" ? "Welcome back!" : "Account created");
    router.push("/dashboard");
    router.refresh();
  };
  return (
    <form
      onSubmit={submit}
      className="space-y-4 rounded-xl border p-6 bg-white dark:bg-zinc-900"
    >
      <input
        className="w-full rounded border p-2 bg-transparent"
        placeholder="Email"
        type="email"
        value={email}
        onChange={(e) => setEmail(e.target.value)}
        required
      />
      <input
        className="w-full rounded border p-2 bg-transparent"
        placeholder="Password"
        type="password"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
        required
        minLength={6}
      />
      <button
        className="w-full rounded bg-zinc-900 text-white dark:bg-zinc-100 dark:text-zinc-900 p-2"
        disabled={loading}
      >
        {loading ? "Please wait..." : mode === "login" ? "Login" : "Sign up"}
      </button>
    </form>
  );
}
