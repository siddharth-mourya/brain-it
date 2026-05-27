import Link from "next/link";
import { AuthForm } from "@/components/auth-form";
export default function SignupPage() {
  return (
    <main className="min-h-screen grid place-items-center p-4">
      <div className="w-full max-w-md space-y-4">
        <h1 className="text-2xl font-semibold">Sign up</h1>
        <AuthForm mode="signup" />
        <p>
          Have account?{" "}
          <Link className="underline" href="/login">
            Login
          </Link>
        </p>
      </div>
    </main>
  );
}
