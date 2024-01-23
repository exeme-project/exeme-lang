---
# https://vitepress.dev/reference/default-theme-home-page
layout: home

hero:
  name: "The Exeme Language"
  text: "An optimised, elegant, and compiled programming language. "
  tagline: Software for the future.
  actions:
    - theme: brand
      text: Guide
      link: /guide/
    - theme: alt
      text: Design
      link: /design/

features:
  - title: Optimised
    icon: 🚀
    details: Exeme is designed to be fast and efficient, with a focus on high quality code.
  - title: Elegant
    icon: 💎
    details: Exeme is designed to have a simple but powerful syntax, with a focus on elegance.
  - title: Compiled
    icon: 📦
    details: Exeme is designed to be portable, with a focus on cross-platform support.
---

<script setup>
import {
  VPTeamPage,
  VPTeamPageTitle,
  VPTeamMembers
} from 'vitepress/theme'

const members = [
  {
    avatar: 'https://www.github.com/skifli.png',
    name: 'Skifli Skiffles',
    title: 'Creator',
    links: [
      { icon: 'github', link: 'https://github.com/skifli' },
      { icon: 'mastodon', link: 'https://mastodon.social/@skifli' }
    ]
  },
]
</script>

<VPTeamPageTitle>
  <template #title>
    Our Team
  </template>
  <template #lead>
    The team behind The Exeme Language.
  </template>
</VPTeamPageTitle>
<VPTeamMembers
  :members="members"
/>
