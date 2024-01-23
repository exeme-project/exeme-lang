import { defineConfig } from 'vitepress'
import footnote from 'markdown-it-footnote'

import pkg from '../../package.json'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "The Exeme Language",
  description: "An optimised, elegant, and compiled programming language.",
  base: '/exeme-lang/',

  lastUpdated: true,
  cleanUrls: true,

  head: [
    ['link', { rel: 'icon', type: 'image/png', href: '/exeme-lang/logo_no_bg.png' }],
  ],

  markdown: {
    config: (md) => {
      md.use(footnote)
    }
  },

  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Design', link: '/design/' },
      { text: 'Developers', link: '/developers/' },
      { text: 'Guide', link: '/guide/' },
      {
        text: pkg.version,
        items: [
          {
            text: 'Changelog',
            link: 'https://github.com/exeme-project/exeme-lang/blob/main/CHANGELOG.md'
          },
          {
            text: 'Contributing',
            link: 'https://github.com/exeme-project/exeme-lang/blob/main/CONTRIBUTING.md'
          }
        ]
      }
    ],
    sidebar: [
      {
        text: 'Design',
        items: [
          { text: 'Introduction', link: '/design/' },
          { text: 'Control Flow', link: '/design/control-flow/' },
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

    logo: "/logo_no_bg.png",
  }
})
