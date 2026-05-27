export type Json =
  | string
  | number
  | boolean
  | null
  | { [key: string]: Json | undefined }
  | Json[];

export type Database = {
  public: {
    Tables: {
      profiles: {
        Row: {
          id: string;
          email: string;
          created_at: string;
        };
        Insert: {
          id: string;
          email: string;
          created_at?: string;
        };
        Update: {
          id?: string;
          email?: string;
          created_at?: string;
        };
        Relationships: [];
      };
      pages: {
        Row: {
          id: string;
          user_id: string;
          parent_id: string | null;
          title: string;
          icon: string | null;
          content: Json | null;
          whiteboard: Json | null;
          created_at: string;
          updated_at: string;
        };
        Insert: {
          id?: string;
          user_id: string;
          parent_id?: string | null;
          title?: string;
          icon?: string | null;
          content?: Json | null;
          whiteboard?: Json | null;
          created_at?: string;
          updated_at?: string;
        };
        Update: {
          id?: string;
          user_id?: string;
          parent_id?: string | null;
          title?: string;
          icon?: string | null;
          content?: Json | null;
          whiteboard?: Json | null;
          created_at?: string;
          updated_at?: string;
        };
        Relationships: [
          {
            foreignKeyName: 'pages_parent_id_fkey';
            columns: ['parent_id'];
            isOneToOne: false;
            referencedRelation: 'pages';
            referencedColumns: ['id'];
          }
        ];
      };
      page_images: {
        Row: {
          id: string;
          page_id: string;
          user_id: string;
          path: string;
          url: string;
          created_at: string;
        };
        Insert: {
          id?: string;
          page_id: string;
          user_id: string;
          path: string;
          url: string;
          created_at?: string;
        };
        Update: {
          id?: string;
          page_id?: string;
          user_id?: string;
          path?: string;
          url?: string;
          created_at?: string;
        };
        Relationships: [
          {
            foreignKeyName: 'page_images_page_id_fkey';
            columns: ['page_id'];
            isOneToOne: false;
            referencedRelation: 'pages';
            referencedColumns: ['id'];
          }
        ];
      };
    };
    Views: Record<string, never>;
    Functions: Record<string, never>;
    Enums: Record<string, never>;
    CompositeTypes: Record<string, never>;
  };
};
