# About

## License

The source code of Qlementine is freely available [on GitHub](https://github.com/oclero/qlementine), and is licensed under the terms of the [MIT License](https://en.wikipedia.org/wiki/MIT_License).

## Authors

- **Olivier Cléro**: Creator and main author. | [email](mailto:oclero@pm.me) | [website](https://www.olivierclero.com) | [github](https://www.github.com/oclero) | [gitlab](https://www.gitlab.com/oclero)
- **Christophe Thiéry**: big contributor.
- A big thanks to people at Abvent, where I gained experience on working on such a library, and people at Filewave, who trusted me by allowing me resources to improve this library.

## MkDocs

### Docs website setup

First, install `mkdocs` and some plugins:

```bash
pip install mkdocs-material
pip install mkdocs-git-revision-date-localized-plugin
```

To run the website locally, go to the website directory, then type:

```bash
mkdocs serve
```

### Build

To build the website to the `/public` directory, ready to be deployed:

```bash
rm -rf ./public
mkdocs build --site-dir public
```

## Docs CI

This documentation website is built automatically with the workflow `docs.yml`, and deployed to Github Pages.
