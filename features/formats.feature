@routing @formats
Feature: Read input data in both .osm and .pbf format

    Background:
        Given the profile "testbot"
    
    Scenario: Formats - OSM format
        Given the import format .osm
        
        Given the node map
         |   | n |   |
         | w | j | e |
         |   | s |   |

        And the ways
         | nodes | oneway |
         | sj    | yes    |
         | nj    | -1     |
         | wj    | -1     |
         | ej    | -1     |

        And the relations
         | type        | way:from | way:to | node:via | restriction  |
         | restriction | sj       | wj     | j        | no_left_turn |

        When I route I should get
         | from | to | route |
         | s    | w  |       |
         | s    | n  | sj,nj |
         | s    | e  | sj,ej |

     Scenario: Formats - PBF format
         Given the import format .pbf

         Given the node map
          |   | n |   |
          | w | j | e |
          |   | s |   |

         And the ways
          | nodes | oneway |
          | sj    | yes    |
          | nj    | -1     |
          | wj    | -1     |
          | ej    | -1     |

         And the relations
          | type        | way:from | way:to | node:via | restriction  |
          | restriction | sj       | wj     | j        | no_left_turn |

         When I route I should get
          | from | to | route |
          | s    | w  |       |
          | s    | n  | sj,nj |
          | s    | e  | sj,ej |