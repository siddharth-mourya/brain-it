'use client';
import { useEffect, useMemo, useState } from 'react';
import { useEditor, EditorContent } from '@tiptap/react';
import StarterKit from '@tiptap/starter-kit';
import Link from '@tiptap/extension-link'; import Image from '@tiptap/extension-image'; import Table from '@tiptap/extension-table'; import TableCell from '@tiptap/extension-table-cell'; import TableHeader from '@tiptap/extension-table-header'; import TableRow from '@tiptap/extension-table-row';
import dynamic from 'next/dynamic';
import { createClient } from '@/lib/supabase/client'; import { toast } from 'sonner';
const Excalidraw = dynamic(() => import('@excalidraw/excalidraw').then(m => m.Excalidraw), { ssr: false });
export function PageWorkspace({ page }: { page: { id: string; title: string; content: any; whiteboard: any; updated_at: string } }) {
 const [title,setTitle]=useState(page.title); const [wb,setWb]=useState<any>(page.whiteboard ?? null); const supabase=useMemo(()=>createClient(),[]);
 const editor=useEditor({ extensions:[StarterKit, Link.configure({openOnClick:false}), Image, Table, TableRow, TableHeader, TableCell], content: page.content ?? '<p>Start writing...</p>' });
 useEffect(()=>{ const interval=setInterval(async()=>{ if(!editor) return; await supabase.from('pages').update({title, content: editor.getJSON(), whiteboard: wb, updated_at:new Date().toISOString()}).eq('id', page.id); },3000); return ()=>clearInterval(interval);},[editor,page.id,supabase,title,wb]);
 const uploadImage=async(file:File)=>{const path=`${page.id}/${Date.now()}-${file.name}`; const {error}=await supabase.storage.from('uploads').upload(path,file,{upsert:false}); if(error) return toast.error(error.message); const {data}=supabase.storage.from('uploads').getPublicUrl(path); editor?.chain().focus().setImage({src:data.publicUrl}).run(); await supabase.from('page_images').insert({page_id:page.id,user_id:(await supabase.auth.getUser()).data.user!.id,path,url:data.publicUrl});};
 return <div className='space-y-4'><input className='w-full text-3xl font-bold bg-transparent outline-none' value={title} onChange={e=>setTitle(e.target.value)} /><div className='rounded border p-4 bg-white dark:bg-zinc-900 editor'><EditorContent editor={editor} /></div><input type='file' accept='image/*' onChange={e=>e.target.files?.[0] && uploadImage(e.target.files[0])} />
 <div className='h-[500px] border rounded bg-white dark:bg-zinc-900'><Excalidraw initialData={wb ?? undefined} onChange={(elements, appState, files)=>setWb({elements, appState, files})} /></div></div>;
}
