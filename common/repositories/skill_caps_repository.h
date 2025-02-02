#ifndef EQEMU_SKILL_CAPS_REPOSITORY_H
#define EQEMU_SKILL_CAPS_REPOSITORY_H

#include "../database.h"
#include "../strings.h"
#include "base/base_skill_caps_repository.h"

class SkillCapsRepository: public BaseSkillCapsRepository {
public:

    /**
     * This file was auto generated and can be modified and extended upon
     *
     * Base repository methods are automatically
     * generated in the "base" version of this repository. The base repository
     * is immutable and to be left untouched, while methods in this class
     * are used as extension methods for more specific persistence-layer
     * accessors or mutators.
     *
     * Base Methods (Subject to be expanded upon in time)
     *
     * Note: Not all tables are designed appropriately to fit functionality with all base methods
     *
     * InsertOne
     * UpdateOne
     * DeleteOne
     * FindOne
     * GetWhere(std::string where_filter)
     * DeleteWhere(std::string where_filter)
     * InsertMany
     * All
     *
     * Example custom methods in a repository
     *
     * SkillCapsRepository::GetByZoneAndVersion(int zone_id, int zone_version)
     * SkillCapsRepository::GetWhereNeverExpires()
     * SkillCapsRepository::GetWhereXAndY()
     * SkillCapsRepository::DeleteWhereXAndY()
     *
     * Most of the above could be covered by base methods, but if you as a developer
     * find yourself re-using logic for other parts of the code, its best to just make a
     * method that can be re-used easily elsewhere especially if it can use a base repository
     * method and encapsulate filters there
     */

	// Custom extended repository methods here
	static std::vector<std::string> GetSkillCapFileLines(Database& db)
	{
		std::vector<std::string> lines;

		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT CONCAT_WS('^', `class_id`, `skill_id`, `level`, `cap`, `class_`) FROM {} ORDER BY `class_id`, `skill_id`, `level` ASC",
				TableName()
			)
		);

		for (auto row : results) {
			lines.emplace_back(row[0]);
		}

		return lines;
	}

	static std::vector<std::string> GetSkillCapFileLinesMulticlass(Database& db)
	{
		std::vector<std::string> lines;

		// Query to get the maximum cap for each skill at each level
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT level, skill_id, MAX(cap) as max_cap FROM {} GROUP BY level, skill_id ORDER BY level, skill_id ASC",
				TableName()
			)
		);

		// Iterate through the results
		for (auto row : results) {
			int level = std::stoi(row[0]);
			int skill_id = std::stoi(row[1]);
			int max_cap = std::stoi(row[2]);

			// Create lines for each class (assuming class IDs are from 1 to 16)
			for (int class_id = 1; class_id <= 16; ++class_id) {
				lines.emplace_back(fmt::format("{}^{}^{}^{}^{}", class_id, skill_id, level, max_cap, class_id));
			}
		}

		return lines;
	}
};

#endif //EQEMU_SKILL_CAPS_REPOSITORY_H
