import nextra from 'nextra'

/**
 * Nextra v4 validates the first argument to `nextra` as a Nextra config object.
 * Passing no argument can fail on Vercel with:
 * "Invalid input: expected object, received undefined".
 */
const withNextra = nextra({})

/** @type {import('next').NextConfig} */
const nextConfig = {}

export default withNextra(nextConfig)
