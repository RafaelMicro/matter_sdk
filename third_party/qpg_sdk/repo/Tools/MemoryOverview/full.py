try:
    from gppy.tools.memory.categorization.generic import Categorization
    import gppy.tools.memory.categorization.feature.zigbee as Zigbee
    import gppy.tools.memory.categorization.feature.matter as Matter
except ImportError:
    try:
        from generic import Categorization
        import matter as Matter
        import zigbee as Zigbee
    except ImportError:
        print("Using outside of Env ?")


class CategorizationFull(Categorization):
    order = [
        "Thread", "CHIP", "Thread/CHIP glue",
        "BLE Base", "BLE", "Mesh", "Zigbee"
    ] + Categorization.order

    Categorization.files.update({
        "CHIP": [],
        "Thread/CHIP glue": Matter.glue_files,
        "Application": Matter.application_files + Matter.cluster_files,
        "Zigbee": Zigbee.base_files,
    })

    Categorization.default.update({
        "CHIP": {
            'P236_CHIP': {},
        },
        "Thread/CHIP glue": {
        },
        "Zigbee": Zigbee.default,
    })

    Categorization.archives.update({
        "Thread/CHIP glue": {"libOpenThread_qpg61": [],
                             "libOpenThreadQorvoGlue": [],
                             },
        "Application": {"libZapGenerated": [], },
        "Zigbee": Zigbee.archives,
    })

    Categorization.combined_archives += ["libCHIP", "libCHIP_qpg61", "libCHIP_qpg6105", "libMatter"]

    Categorization.ignore_folders += ["lib"]  # P236_qvCHIP library
