import { NotFoundPage } from 'nextra-theme-docs'

export default function NotFound() {
  return (
    <NotFoundPage content={null}>
      <h1>404: Page Not Found</h1>
      <p>The requested DeskBot documentation page could not be found.</p>
    </NotFoundPage>
  )
}
