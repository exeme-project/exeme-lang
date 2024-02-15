import starlight from '@astrojs/starlight';
import {defineConfig} from 'astro/config';

// https://astro.build/config
export default defineConfig({
  site : "https://exeme-project.github.io",
  base : "/exeme-lang",
  integrations : [
    starlight({
      title : 'The Exeme Language',
      logo : {
        src : "./src/assets/logo.png",
      },
      social : {
        github : 'https://github.com/exeme-project/exeme-language',
      },
      sidebar : [
        {
          label : 'Guides',
          autogenerate : {directory : 'guides'},
        },
        {
          label : 'Language Specification',
          autogenerate : {directory : 'language-specification'},
        },
      ],
      editLink : {
        baseUrl : "https://github.com/exeme-project/exeme-lang/edit/main/docs/",
      },
      lastUpdated : true,
      pagination : true,
      favicon : "./src/assets/logo.png"
    }),
  ],
});
