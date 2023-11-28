import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "The Exeme Language",
  description: "An optimised, elegant, and compiled programming language.",
  base: '/exeme-lang/',

  lastUpdated: true,
  cleanUrls: true,

  head: [
    ['link', {
      rel: 'icon', type: 'image/svg+xml', href: 'https://avatars.githubusercontent.com/u/124382299?s=400&u=0a0596688b27c5b40b2df9521d6fb8460904b3ef&v=4'
    }],
    ['link', { rel: 'icon', type: 'image/png', href: 'https://avatars.githubusercontent.com/u/124382299?s=400&u=0a0596688b27c5b40b2df9521d6fb8460904b3ef&v=4' }],
  ],

  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Design', link: '/design/' },
      { text: 'Developers', link: '/developers/' },
      { text: 'Guide', link: '/guide/' }
    ],
    sidebar: [
      {
        text: 'Design',
        items: [
          { text: 'Introduction', link: '/design/' },
          { text: 'Functions', link: '/design/functions/' },
          { text: 'Namespaces', link: '/design/namespaces/' },
          { text: 'Packages', link: '/design/packages/' },
        ],
      },
      {
        text: 'Developers',
        items: [
          { text: 'Introduction', link: '/developers/' },
        ]
      },
      {
        text: 'Guide',
        items: [
          { text: 'Introduction', link: '/guide/' },
          { text: 'Error Codes Index', link: '/guide/error-codes-index/' },
        ]
      }
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/exeme-project/exeme-lang' },
    ],


    editLink: {
      pattern: 'https://github.com/exeme-project/exeme-lang/edit/main/docs/:path',
      text: 'Edit this page on GitHub'
    },

    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright © 2023 - Present The Exeme Project'
    },

    search: {
      provider: 'local'
    },

    logo: "https://avatars.githubusercontent.com/u/124382299?s=400&u=0a0596688b27c5b40b2df9521d6fb8460904b3ef&v=4",
  }
})
