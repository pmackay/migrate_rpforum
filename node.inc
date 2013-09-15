<?php

/**
 * Import forum topics from RP Forum to core forum nodes.
 */
class RPForumTopicMigration extends Migration {
  // static $phpbb_type = 'Topic';
  public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate individual forum topics from RP Forum.');
    // $this->dependencies = array($this->getMigrationName('User'), $this->getMigrationName('Forums'));
    $this->source = new MigrateSourceSQL($this->query(), array(), NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationNode('forum');//,
    //   array('text_format' => $this->importFormat)
    // );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          // 'alias' => 'p'
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Put NULL in here due to error http://drupal.org/node/839770
    // $this->addFieldMapping('nid', 'nid');
    // $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid');
    // $this->addFieldMapping('vid', 'vid');
      // ->sourceMigration($this->getMigrationName('User'))->defaultValue(1);
    $this->addFieldMapping('taxonomy_forums')->defaultValue(30)
      // ->sourceMigration($this->getMigrationName('Forums'))
      ->arguments(array('source_type' => 'tid'));
//    $this->addFieldMapping('hostname', 'poster_ip');
    // $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'field_body2_value')
      ->arguments(array('format' => 'panopoly_html_text'));
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'changed');
    // $this->addFieldMapping('language')->defaultValue($this->defaultLanguage);
    // $this->addFieldMapping('sticky', 'topic_type');
    // $this->addFieldMapping('totalcount','topic_views');
    // $this->addFieldMapping(NULL, 'bbcode_uid')->issueGroup(t('DNM')); // Used to santise body text

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('is_new', 'promote',
      'revision', 'language', 'sticky', 'revision_uid', 'path','tnid'));

    // $this->addUnmigratedDestinations(array(
    //   'is_new', 'revision', 'revision_uid', 'promote', 'comment', 'path', 'pathauto'
    // ));
  }

  // protected function taxonomyMapping

  /**
   * Method to return the query used in the import.
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
    $query = Database::getConnection('default', 'd6.hernehill')->select('content_type_rp_forum_post', 'rp');
    $query->leftJoin('node', 'n', 'rp.nid = n.nid');
    $query->fields('n', array('nid', 'title', 'uid', 'created', 'changed'));
    // $query->fields('p', array(
    //     'forum_id', 'post_id', 'poster_id', //'poster_ip',
    //     'post_time', 'post_edit_time', 'post_approved',
     //     'post_subject', 'post_text', 'bbcode_uid',
    //     'post_edit_reason', 'post_edit_user', 'post_edit_count'));
    $query->fields('rp', array('vid','nid','field_body2_value','field_parent_node_value'));
    $query->condition('field_parent_node_value', -1);
    $query->condition('rp.nid', 442);
    //   ->where('p.post_id = t.topic_first_post_id')
    //   ->orderBy('post_time');
    return $query;
  }
}

/**
 * Import forum reply posts from RP Forum to core forum comments.
 */
class RPForumPostMigration extends Migration {
  // static $phpbb_type = 'Topic';
  public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate individual forum posts from RP Forum.');
    // $this->dependencies = array($this->getMigrationName('User'), $this->getMigrationName('Forums'));
    $this->source = new MigrateSourceSQL($this->query(), array(), NULL, array('map_joinable' => FALSE));
    $this->dependencies =  array('RPForumTopic');
    $this->destination = new MigrateDestinationComment('comment_node_forum');//,
    //   array('text_format' => $this->importFormat)
    // );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          // 'alias' => 'p'
        )
      ),
      MigrateDestinationComment::getKeySchema()
    );

    // Put NULL in here due to error http://drupal.org/node/839770
    $this->addFieldMapping('nid', 'field_parent_node_value')
      ->sourceMigration('RPForumTopic');;
    $this->addFieldMapping('uid', 'uid');
    // $this->addFieldMapping('vid', 'vid');
      // ->sourceMigration($this->getMigrationName('User'))->defaultValue(1);
    // $this->addFieldMapping('taxonomy_forums')->defaultValue(30)
      // ->sourceMigration($this->getMigrationName('Forums'))
      // ->arguments(array('source_type' => 'tid'));
//    $this->addFieldMapping('hostname', 'poster_ip');
    // $this->addFieldMapping('status', 'status');
    // $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('subject', 'title');
    $this->addFieldMapping('comment_body', 'field_body2_value')
      ->arguments(array('format' => 'panopoly_html_text'));
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'changed');
    // $this->addFieldMapping('language')->defaultValue($this->defaultLanguage);
    // $this->addFieldMapping('sticky', 'topic_type');
    // $this->addFieldMapping('totalcount','topic_views');
    // $this->addFieldMapping(NULL, 'bbcode_uid')->issueGroup(t('DNM')); // Used to santise body text

    // Unmapped destination fields
    // $this->addUnmigratedDestinations(array('is_new', 'promote',
    //   'revision', 'language', 'sticky', 'revision_uid', 'path','tnid'));
    $this->addUnmigratedDestinations(array(
      'pid', 'path', 'thread', 'name', 'mail', 'homepage'
    ));
    // $this->addUnmigratedDestinations(array(
    //   'is_new', 'revision', 'revision_uid', 'promote', 'comment', 'path', 'pathauto'
    // ));
  }

  // protected function taxonomyMapping

  /**
   * Method to return the query used in the import.
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
    $query = Database::getConnection('default', 'd6.hernehill')->select('content_type_rp_forum_post', 'rp');
    $query->leftJoin('node', 'n', 'rp.nid = n.nid');
    $query->fields('n', array('nid', 'title', 'uid', 'created', 'changed'));
    // $query->fields('p', array(
    //     'forum_id', 'post_id', 'poster_id', //'poster_ip',
    //     'post_time', 'post_edit_time', 'post_approved',
     //     'post_subject', 'post_text', 'bbcode_uid',
    //     'post_edit_reason', 'post_edit_user', 'post_edit_count'));
    $query->fields('rp', array('vid','nid','field_body2_value','field_parent_node_value'));
    // $query->condition('field_parent_node_value', -1, '<>');
    $query->condition('field_parent_node_value', 442);
    //   ->where('p.post_id = t.topic_first_post_id')
    //   ->orderBy('post_time');
    return $query;
  }
}
