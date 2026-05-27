import Link from 'next/link';
import { AuthForm } from '@/components/auth-form';
export default function LoginPage(){return <main className='min-h-screen grid place-items-center p-4'><div className='w-full max-w-md space-y-4'><h1 className='text-2xl font-semibold'>Login</h1><AuthForm mode='login'/><p>New? <Link className='underline' href='/signup'>Create account</Link></p></div></main>}
