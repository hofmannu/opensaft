import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "opensaft",
  description: "Open source SAFT for optoacoustic imaging",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Physics', link: '/physics' },
      { text: 'Software', link: '/software' },
      { text: 'Hardware', link: '/hardware' },
      { text: 'Literature', link: '/literature' },
    ],
    outline: [2, 4],
    socialLinks: [
      { icon: 'github', link: 'https://github.com/hofmannu/opensaft' }
    ],
    footer: {
      message: 'Created by Urs Hofmann',
      copyright: 'Copyright © 2024 Urs Hofmann'
    },

    lastUpdated: {
      text: 'Updated at',
      formatOptions: {
        dateStyle: 'full',
        timeStyle: 'medium'
      }
    },

    i18nRouting: true,
    search: {
      provider: 'local'
    },

    editLink: {
      pattern: 'https://github.com/hofmannu/opensaft/edit/main/blog/:path',
    },

    markdown: {
      math: true
    },
  }
  base: '/opensaft/',
})
