---
# https://vitepress.dev/reference/default-theme-home-page
layout: home

hero:
  name: "opensaft"
  text:
  tagline: An open source implementation of the SAFT technique for optoacoustic imaging
  image:
    src: /logo.svg
    alt: opensaft-logo
  actions:
    - theme: alt
      text: Physics
      link: /physics
    - theme: alt
      text: Software
      link: /software
    - theme: alt
      text: Hardware
      link: /hardware
    - theme: alt
      text: Literature
      link: /literature

features:
  - title: Open source
    details: This repository is and will remain open source to allow contributions of others as well as adaptions to your needs.
  - title: Performant
    details: The code is optimized for performance using CUDA or can run on multicore CPU if you don't have a GPU at hand
  - title: Informative
    details: Through this documentation you can learn more about the technique and how to use it as well as potential hardware implementations
  - title: Simulations
    details: We are planning to add a simulation toolbox to generate test datasets based on FDTD simulations.

